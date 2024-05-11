/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdlib.h"
#include "printf.h"
#include "string.h"
#include "../../API/Menu.h"
#include "../inc/gpio.h"
#include "../inc/tim.h"
#include "../inc/usart.h"
#include "../inc/ESP01.h"

#include "w25qxx.h"
#include "DS18B20.h"
#include "MPU6050.h"
#include "GUI.h"
#include "touch.h"
#include <cmsis_os2.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define TH            90.0f
#define SEV              9
#define ALARM_MAX       60
#define UPLOAD_MAX 10000.0f


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

float vPitch[pPLOT_NUM]  = {0.0f};
float  vTemp[tPLOT_NUM]  = {0.0f};
float  vRoll[rPLOT_NUM]  = {0.0f};
float   vYaw[yPLOT_NUM]  = {0.0f};

PARA_list   para  = {
       5,         /*    Sensitivity level (0~9)         */
      30,         /*    Alarm duration(s) (0~60)        */
  1000.0f,         /*    Upload interval (100~10000ms)   */
    30.0f         /*    temp thresh (0~90) degree       */
};

PARA_list set;

Temperature gtemp = {
    0.0f,         /*  new temperature    */
    0.0f,         /*  old temperature    */
       0,         /*  temperature update */
       0,         /*  temperature warn   */
       0,         /*  temperature cnt    */
    30.0f         /*  temperature thresh */
};

MPUpacket mpu = {
       1,         /*    ax    */ 
       0,         /*    ay    */
      -1,         /*    az    */
       1,         /*    gx    */
       0,         /*    gy    */
      -1,         /*    gz    */
    0.0f,         /*   fAX    */
    0.0f,         /*   fAY    */
    0.0f,         /*   fAZ    */
       0,         /*    ok    */ 
       0,         /*  warn    */ 
       1,         /*  update  */
       0          /*  counter */ 
};

Page_t Book = {
        1,        /*    cur  page     */
      127,        /*    last page     */
        0,        /*    skip button   */
TEMP_FIRST,       /*    Priority      */
TEMP_CURVE,       /*    prev-Priority */
        1,        /*    turn page     */
        0,        /*  PARA SET-order  */
        0,        /*  fetch thresh    */
        0         /*  to kill beep    */
};

WifiPacket SensorPack = {
        1,        /*    Deliever to TCP     */
   100.0f,        /*    Deliever interval   */
      "",         /*    Sensor Data Buffer  */
};

uint32_t current_tick = 0;
uint32_t beeptick = 0;
uint32_t warntick = 0;
uint16_t key = 0;
uint16_t num[4];

/* USER CODE END Variables */
/* Definitions for TempTask */
osThreadId_t TempTaskHandle;
const osThreadAttr_t TempTask_attributes = {
  .name = "TempTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for KeyTask */
osThreadId_t KeyTaskHandle;
const osThreadAttr_t KeyTask_attributes = {
  .name = "KeyTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for MPUTask */
osThreadId_t MPUTaskHandle;
const osThreadAttr_t MPUTask_attributes = {
  .name = "MPUTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for GUITask */
osThreadId_t GUITaskHandle;
const osThreadAttr_t GUITask_attributes = {
  .name = "GUITask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for WifiTask */
osThreadId_t WifiTaskHandle;
const osThreadAttr_t WifiTask_attributes = {
  .name = "WifiTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal2,
};
/* Definitions for DefaultTask */
osThreadId_t DefaultTaskHandle;
const osThreadAttr_t DefaultTask_attributes = {
  .name = "DefaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void GUI_Start();
void Beep(uint8_t tune, uint16_t time);
void Beep_mpu(uint8_t tune, uint16_t time);
void BeepDone(void);
/* USER CODE END FunctionPrototypes */

void StartTempTask(void *argument);
void StartKeyTask(void *argument);
void StartMPUTask(void *argument);
void StartGUITask(void *argument);
void StartWifiTask(void *argument);
void StartMainTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of TempTask */
  TempTaskHandle = osThreadNew(StartTempTask, NULL, &TempTask_attributes);

  /* creation of KeyTask */
  KeyTaskHandle = osThreadNew(StartKeyTask, NULL, &KeyTask_attributes);

  /* creation of MPUTask */
  MPUTaskHandle = osThreadNew(StartMPUTask, NULL, &MPUTask_attributes);

  /* creation of GUITask */
  GUITaskHandle = osThreadNew(StartGUITask, NULL, &GUITask_attributes);

  /* creation of WifiTask */
  WifiTaskHandle = osThreadNew(StartWifiTask, NULL, &WifiTask_attributes);

  /* creation of DefaultTask */
  DefaultTaskHandle = osThreadNew(StartMainTask, NULL, &DefaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartTempTask */
/**
  * @brief  Function implementing the TempTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTempTask */
void StartTempTask(void *argument)
{
  /* USER CODE BEGIN StartTempTask */
  /* Infinite loop */

  ds18b20_init();

  for(;;)
  {
      if ( Book.Priority == TEMP_FIRST || Book.Priority == WIFI_ESP || Book.Priority == PARA_SET) 
      {

          // osThreadFlagsWait(0x0001, osFlagsWaitAny, 5);

          osDelay(20);
          gtemp.new  = ds18b20_read();

          if ( gtemp.new < TH )
          {
              if ( gtemp.new >= (float)gtemp.thresh ) 
              { 
                  gtemp.warn = 1;
              }
          }
      }
      else if ( Book.Priority == TEMP_CURVE ) 
      {
          osThreadFlagsWait(0x0001, osFlagsWaitAny, 2);
          
          osDelay(100);
          gtemp.new  = ds18b20_read();

          if ( gtemp.new < TH )
          {
              if ( gtemp.cnt < tPLOT_NUM )  
                vTemp[gtemp.cnt++] = gtemp.new;
              else 
              {
                  memcpy((void *)vTemp, (void *)(vTemp +1), sizeof(vTemp[0]) * (tPLOT_NUM - 1));
                  vTemp[tPLOT_NUM -1] = gtemp.new;
              }

              if ( gtemp.new >= (float) gtemp.thresh ) 
              { 
                  gtemp.warn = 1;
              }
          }
      }
      if ( ( gtemp.warn || mpu.warn ) && para.alarm > 0 ) 
      {
          if ( 0 == warntick )
              warntick = osKernelGetTickCount();
          else if ( para.alarm == 0 || osKernelGetTickCount() >= warntick + para.alarm * 1000 || !Book.set) 
          {
              gtemp.warn = 0;
              mpu.warn = 0;
              warntick = 0;
          }
          else
          {
              uint32_t tic = warntick + 30000 - osKernelGetTickCount();
              num[0] =  (tic / 10000) % 10;
              num[1] =  (tic / 1000) % 10;
              num[2] =  (tic / 100) % 10;
              num[3] =  (tic / 10) % 10;

              if ((num[2] == 1 || num[2] == 2 || num[2] == 7) && ( gtemp.warn && !mpu.warn && !Book.killbeep))
              {
                  Beep(num[2],100);
              }
              else if (( num[2] == 7 || num[2] == 2 || num[2] == 5 ) && ( !gtemp.warn && mpu.warn && !Book.killbeep ))
              {
                  Beep_mpu(num[2],100);
              }
          } 
      }
      else
        num[0] = num[1] = num[2] = num[3] = ' ';
    

      BeepDone();
      osDelay(1);
  }
  /* USER CODE END StartTempTask */
}

/* USER CODE BEGIN Header_StartKeyTask */
/**
* @brief Function implementing the KeyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartKeyTask */
void StartKeyTask(void *argument)
{
  /* USER CODE BEGIN StartKeyTask */
  /* Infinite loop */
  for(;;)
  {
      if ( (gtemp.warn || mpu.warn ) && para.alarm > 0 )
      {
        key = ScanKey(1);
        current_tick = 0;
      }
		  else 
      {
        key = ScanKey(0);
        Book.killbeep = 0;
      }
      if ( key > 0 )
      {
          if ( key == KSUM )
          {
                current_tick = osKernelGetTickCount();

                while ( key == KSUM )
                {
                    if ( osKernelGetTickCount() >= current_tick + 3000 )
                    {
                        Book.killbeep = 1;
                        key = K_IDIE;
                        break;
                    }
                    key = ScanKey(1);
                }
          }
          switch ( key )
          {
              case K1_Pin:

                Book.cur = MENU[Book.cur].up;

                Book_Priorswitch (Book.cur);     
                osDelay(300);

              break;

              case K4_Pin:

                Book.cur = MENU[Book.cur].down;

                Book_Priorswitch (Book.cur);    
                osDelay(300);

              break;

              case K2_Pin:

                if ( Book.Priority == WIFI_ESP ) 
                {
                    SensorPack.button = 1; 
                    // transmitting SensorDataPack;
                }
                else if ( Book.Priority == PARA_SET && (gtemp.warn != 1 && mpu.warn != 1) )
                {
                  
                    osDelay(300);

                    switch ( Book.gpara )
                    {
                      case 0:

                          if ( para.maxtemp < TH )
                          {
                              para.maxtemp += 0.1f;
                          }
                          else
                          {
                              para.maxtemp = (float)TH;
                          }

                      break;

                      case 1:

                          if ( para.sensitivity < SEV )
                          {
                              para.sensitivity += 1;
                          }
                          else
                          {
                              para.sensitivity = SEV;
                          }

                      break;

                      case 2:
                          
                          if ( para.alarm < ALARM_MAX )
                          {
                              para.alarm   += 1;
                          }
                          else
                          {
                              para.alarm = ALARM_MAX;
                          }

                      break;

                      case 3:

                          if ( para.upload < UPLOAD_MAX )
                          {
                              para.upload  += 100;
                          }
                          else
                          {
                              para.upload = UPLOAD_MAX;
                          }

                      break;

                      default:
                      break;
                    }

                }
                else
                {
                    Book.cur = MENU[Book.cur].left;  

                    Book_Priorswitch (Book.cur);  
                    osDelay(300);
                }


              break;

              case K3_Pin:   

                if ( Book.Priority == WIFI_ESP ) 
                { 
                    SensorPack.button = 0;
                    // close
                }
                else if ( Book.Priority == PARA_SET && (gtemp.warn != 1 && mpu.warn != 1) )
                {

                    osDelay(300);

                    switch ( Book.gpara )
                    {
                      case 0:

                          if ( para.maxtemp > 0.0f )
                          {
                              para.maxtemp -= 0.1f;
                          }
                          else
                          {
                              para.maxtemp = 0.0f;
                          }

                      break;

                      case 1:

                          if ( para.sensitivity > 0 )
                          {
                              para.sensitivity -= 1;
                          }
                          else
                          {
                              para.sensitivity = 0;
                          }

                      break;

                      case 2:
                          
                          if ( para.alarm > 0 )
                          {
                              para.alarm -= 1;
                          }
                          else
                          {
                              para.alarm = 0;
                          }

                      break;

                      case 3:

                          if ( para.upload > 100 )
                          {
                              para.upload -= 100;
                          }
                          else
                          {
                              para.upload = 100;
                          }

                      break;
                      
                      default:
                      break;
                    }

                }
                else
                {

                    Book.cur = MENU[Book.cur].right; 

                    Book_Priorswitch (Book.cur);   
                    osDelay(300);

                }
                
              break;

              case K5_Pin:

                if ( Book.Priority != WIFI_ESP && Book.Priority != PARA_SET ) 
                {
                    Book.cur = MENU[Book.cur].enter;

                    Book_Priorswitch (Book.cur);   
                    osDelay(300);
                }

                else if ( Book.Priority == PARA_SET )
                {
                    Book.gpara += 1;
                    Book.gpara %= 4;
                    osDelay(500);
                }

                else if ( Book.Priority == WIFI_ESP )
                {
                    if ( Book.cur != 11)   { InitEsp01(&huart6);}
                    else                   { g_esp01.Sendbytes = 0; g_esp01.Recvbytes = 0; }

                }

              break;

              case K6_Pin:

                if ( Book.Priority != WIFI_ESP )
                  Book.cur = MENU[Book.cur].back;
                else 
                  Book.cur = MENU[Book.cur].right;

                Book_Priorswitch (Book.cur);   
                osDelay(300);

              break;

              case K_IDIE:
                printf("success brake beep!\n");
                osDelay(100);
              break;
              default:
              break;
        }

        if ( Book.Priority == PARA_SET && (key == K2_Pin || key == K3_Pin) )
        {
            if ( memcpy(&set, &para, sizeof(para) ) )
            {
                printf("Save %.1f°Ê, %d, %d√Î, %.1f∫¡√Î\n", para.maxtemp, para.sensitivity,
                        para.alarm, para.upload);
                W25QXX_Write( (uint8_t *)&para , 0, sizeof(para) );       
                gtemp.thresh = (float)para.maxtemp;
                SensorPack.interval = para.upload; 
            }
        }

      }

    osDelay(1);
  }
  /* USER CODE END StartKeyTask */
}

/* USER CODE BEGIN Header_StartMPUTask */
/**
* @brief Function implementing the MPUTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMPUTask */
void StartMPUTask(void *argument)
{
  /* USER CODE BEGIN StartMPUTask */
  /* Infinite loop */
  uint32_t tick = 0;
  mpu.ok = MPU_init();
  
  uint8_t cnt = 0;
  while ( cnt++ < 3 && !mpu.ok)
  {
    osDelay(500);
    mpu.ok = MPU_init();
  }
  InitEsp01(&huart6);

  W25QXX_Init();

  W25QXX_Read(&set, 0, sizeof(set));
  if (( set.maxtemp  >= 0.0f && set.maxtemp <= TH       ) &&
      ( set.sensitivity >= 0 && set.sensitivity <= SEV  ) &&
      ( set.alarm       >= 0 && set.alarm   <= ALARM_MAX) &&
      ( set.upload   >= 100.0f  && set.upload <= UPLOAD_MAX)  )
  {
        memcpy(&para, &set, sizeof(set));
        printf("Load %.1f°Ê, %d, %d√Î, %.1f∫¡√Î\n", para.maxtemp, para.sensitivity,
                            para.alarm, para.upload);
        gtemp.thresh = (float)para.maxtemp;
        SensorPack.interval = para.upload;
        Book.set = 1;
  }
  /* Infinite loop */
  for(;;)
  {    
      if ( mpu.ok && ( Book.Priority == MPU_FIRST || Book.Priority == WIFI_ESP || Book.Priority == PARA_SET || Book.Priority == TEMP_FIRST ) )
      {
          // osThreadFlagsWait(0x0002, osFlagsWaitAny, 5);

          MPU_getdata();
              mpu.ax = ax;
              mpu.ay = ay;
              mpu.az = az;
              mpu.gx = gx;
              mpu.gy = gy;
              mpu.gz = gz;
              mpu.fAX = fAX;
              mpu.fAY = fAY;
              mpu.fAZ = fAZ;
              mpu.update = 1;

          // SWV output
          // printf("MPU Data:%4.1f %4.1f %4.1f, %5d %5d %5d, %5d %5d %5d\n", fAX, fAY, fAZ, ax, ay, az, gx, gy, gz);
      }

      else if ( mpu.ok && ( Book.Priority == MPU_PITCH || Book.Priority == MPU_ROLL || Book.Priority == MPU_YAW ) )
      {
          
          osThreadFlagsWait(0x0002, osFlagsWaitAny, 2);

          MPU_getdata();
              mpu.ax = ax;
              mpu.ay = ay;
              mpu.az = az;
              mpu.gx = gx;
              mpu.gy = gy;
              mpu.gz = gz;
              mpu.fAX = fAX;
              mpu.fAY = fAY;
              mpu.fAZ = fAZ;

          if ( Book.Prev_Priority != Book.Priority ) { mpu.cnt = 0; }

          switch ( Book.Priority )
          {
              case MPU_PITCH:

                  if ( mpu.cnt < tPLOT_NUM )
                    vPitch[mpu.cnt++] = fAX;

                  else 
                  {
                    memcpy( (void *) vPitch , (void *) ( vPitch + 1 ) , sizeof( vPitch[0] ) * ( pPLOT_NUM - 1 ) );
                    vPitch[pPLOT_NUM -1] = fAX;
                  }

                  Book.Prev_Priority = MPU_PITCH;

              break;
              case  MPU_ROLL:

                  if ( mpu.cnt < rPLOT_NUM )
                    vRoll[mpu.cnt++] = fAY;
                  else
                  {
                    memcpy( (void *)vRoll , (void *) ( vRoll + 1 ) , sizeof( vRoll[0] ) * ( rPLOT_NUM - 1 ) );
                    vRoll[rPLOT_NUM -1] = fAY;
                  }

                  Book.Prev_Priority = MPU_ROLL;

              break;
              case   MPU_YAW:

                  if ( mpu.cnt < yPLOT_NUM )
                    vYaw[mpu.cnt++] = fAZ;
                  else
                  {
                    memcpy( (void *)vYaw , (void *) ( vYaw + 1 ), sizeof( vYaw[0] ) * ( yPLOT_NUM - 1 ) );
                    vYaw[yPLOT_NUM -1] = fAZ;
                  }

                  Book.Prev_Priority = MPU_YAW;

              break;            
          }
          osDelay(50);
      }

      if ( gx * gx + gy * gy + gz * gz > 5000 * ( 10 - para.sensitivity ) && para.alarm > 0 ) 
      {
          mpu.warn = 1;
      } 

      osDelay(1);
  }
  /* USER CODE END StartMPUTask */
}

/* USER CODE BEGIN Header_StartGUITask */
/**
* @brief Function implementing the GUITask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGUITask */
void StartGUITask(void *argument)
{
  /* USER CODE BEGIN StartGUITask */
  /* Infinite loop */

  HAL_TIM_Base_Start(&htim3);
  GUI_Init();
  GUI_Start();

  // printf("Hello STM32F407!\n");
  static uint16_t Subprioity = TEMP_FIRST;
  // osThreadFlagsSet(TempTaskHandle , 0x0001);
  // osDelay(2U);

  for(;;)
  {
      if ( Book.last != Book.cur ) 
      {

          Book.update = 1;
          gtemp.update = 0;
          mpu.update   = 1;
          Book_Pageturn ( Book.cur, gtemp, mpu );
          Book.last = Book.cur;

      }

      else 
      {

          Book.update = 0;
          Book.button = 0;

          switch ( Book.Priority )
          {
              case TEMP_FIRST:

                  if ( gtemp.new != gtemp.old ) 
                  {
                    gtemp.update = 1;
                    Book_Pageturn ( Book.cur, gtemp, mpu );
                    gtemp.old = gtemp.new;
                  } 

                  else if ( gtemp.new == gtemp.old ) 
                  {
                    gtemp.update = 0;
                    // osThreadFlagsSet(TempTaskHandle , 0x0001);

                    osDelay(2U);
                  }

              break;

              case  MPU_FIRST:

                  if ( mpu.update ) 
                  {
                    Book_Pageturn ( Book.cur, gtemp, mpu );
                    mpu.update = 0;
                  }
                  // if ( !mpu.update ) 
                  // {
                  //   // osThreadFlagsSet ( MPUTaskHandle , 0x0002 );

                  //   osDelay(2U);
                  // }

              break;

              case  TEMP_CURVE:

                  osThreadFlagsSet ( TempTaskHandle , 0x0001 );
                  osDelay(2U);
                  Book_Pageturn ( Book.cur, gtemp, mpu );

              break;

              case   MPU_YAW:

                  osThreadFlagsSet ( MPUTaskHandle , 0x0002 );
                  osDelay(2U);
                  Book_Pageturn ( Book.cur, gtemp, mpu );

              break;

              case   MPU_ROLL:

                  osThreadFlagsSet ( MPUTaskHandle , 0x0002 );
                  osDelay(2U);
                  Book_Pageturn ( Book.cur, gtemp, mpu );

              break;

              case   MPU_PITCH:

                  osThreadFlagsSet ( MPUTaskHandle , 0x0002 );
                  osDelay(2U);
                  Book_Pageturn ( Book.cur, gtemp, mpu );

              break;

              case  WIFI_ESP:

                  switch (Subprioity)
                  {
                    case TEMP_FIRST:

                        if ( gtemp.new != gtemp.old ) 
                        {
                          gtemp.update = 1;
                          gtemp.old = gtemp.new;
                        } 

                        else if ( gtemp.new == gtemp.old ) 
                        {
                          gtemp.update = 0;
                          // osThreadFlagsSet(TempTaskHandle , 0x0001);

                          osDelay(2U);
                        }

                        Subprioity = MPU_FIRST;
                        osDelay(10);
                    break;
                    case MPU_FIRST:
                    
                        if ( mpu.update ) 
                        {
                          Book_Pageturn ( Book.cur, gtemp, mpu );
                          mpu.update = 0;
                        }
                        // if ( !mpu.update ) 
                        // {
                        //   osThreadFlagsSet ( MPUTaskHandle , 0x0002 );

                        //   osDelay(2U);
                        // }

                        Subprioity = TEMP_FIRST;
                        osDelay(10);

                    default:
                    break;
                  }
      
              break;

              case   PARA_SET:

                  Book_Pageturn ( Book.cur, gtemp, mpu );
                  osDelay(100);

              break;
              default:
              break;
          }
      }

      if ( Book.cur == 0 ) { Book.cur = 1; }

      BeepDone();
      osDelay(100);
  }
  /* USER CODE END StartGUITask */
}

/* USER CODE BEGIN Header_StartWifiTask */
/**
* @brief Function implementing the WifiTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartWifiTask */
void StartWifiTask(void *argument)
{
  /* USER CODE BEGIN StartWifiTask */
  /* Infinite loop */

  for(;;)
  {
    if ( EspRxDataOk() )
    {
        if ( g_esp01.rxdata.rx_len > 0)
        {
          char *pstr = (char *) (g_esp01.rxdata.rx_buf);

          if ( strstr(pstr, "UPON") == pstr )
              SensorPack.button = 1;

          else if ( strstr(pstr, "UPOFF") == pstr )
              SensorPack.button = 0;
              
          else if (( strstr(pstr, "GETPARA") == pstr ) && ( SensorPack.button == 1 ))
          { 
            char upstr[50];
            sprintf(upstr, "P:%d,%d,%d,%d\n",(int)para.maxtemp, para.sensitivity, para.alarm,(int)para.upload);
            SendEspStr(upstr);
          }
          else if ( strstr(pstr, "PSET:") == pstr )
          {
            PARA_list set_s;
            char *p = pstr + 5;
            set_s.maxtemp = atoi(p);
            p = strstr(p, ",");
            if ( p > pstr )
            {
              set_s.sensitivity = atoi(p + 1);
              p = strstr(p + 1 , ",");
              if ( p > pstr )
              {
                set_s.alarm = atoi(p + 1);
                p = strstr(p + 1, ",");
                if ( p > pstr )
                {
                  set_s.upload = atoi(p + 1);
                  if (( set_s.maxtemp     >= 0 && set_s.maxtemp <= 90     ) &&
                      ( set_s.sensitivity >= 0 && set_s.sensitivity <= 9  ) &&
                      ( set_s.alarm       >= 0 && set_s.alarm <= 60       ) &&
                      ( set_s.upload      >= 100.0  && set_s.upload <= 10000 ))
                  {
                      memcpy(&para, &set_s, sizeof(set_s));
                      printf("Save %.1f°Ê, %d, %d√Î, %.1f∫¡√Î\n", para.maxtemp, para.sensitivity,
                      para.alarm, para.upload);
                      W25QXX_Write( (uint8_t *)&para , 0, sizeof(para) );    
                      gtemp.thresh = (float)para.maxtemp;
                      SensorPack.interval = para.upload; 
                  }
                }
              }
            }
          }
        }
      // Ω” ’ ˝æ›¥¶¿Ì
    }
    osDelay(1);
  }
  /* USER CODE END StartWifiTask */
}

/* USER CODE BEGIN Header_StartMainTask */
/**
* @brief Function implementing the DefaultTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMainTask */
void StartMainTask(void *argument)
{
  /* USER CODE BEGIN StartMainTask */
  /* Infinite loop */
  for(;;)
  {
    if ( SensorPack.button && g_esp01.bConnect == 3 ) 
    {
          osDelay(SensorPack.interval);
          sprintf(SensorPack.buf, "T:%5.1f, A:%6d %6d %6d, G:%6d %6d %6d, F:%6.1f %6.1f %6.1f, W:%d\n", 
                  gtemp.new, mpu.ax, mpu.ay, mpu.az, mpu.gx, mpu.gy, mpu.gz, mpu.fAX, mpu.fAY, mpu.fAZ, gtemp.warn + mpu.warn * 2);
          SendEspStr(SensorPack.buf);
    }
    osDelay(1);
  }
  /* USER CODE END StartMainTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

static void GUI_Start()
{    
	  GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
    GUI_SetBkColor(GUI_BLACK);
    for ( int i = 0; i < 61; i++ ) {

      if ( Book.button ) 
      {
          GUI_Clear();
          GUI_SetBkColor(GUI_GRAY_D0);
          GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
          break;
      }

      if ( i % 5 == 0 && i!=60 )
        GUI_ClearRect(88,48,216,176);
      else if ( i % 5 == 1)
        GUI_DrawBitmap(&bmfirealarm30, 88, 48);
      else if ( i % 5 == 2)
        GUI_DrawBitmap(&bmfirealarm60, 88, 48);
      else if ( i % 5 == 3)
        GUI_DrawBitmap(&bmfirealarm82, 88, 48);
      else if ( i % 5 == 4)
        GUI_DrawBitmap(&bmfirealarm, 88, 48);

      if ( mpu.ok ) {
        
        if ( i % 10 == 3) {
          GUI_ClearRect(250,170,310,190);
          GUI_SetColor(GUI_LIGHTRED);
          GUI_SetFont(GUI_FONT_24B_1);
          GUI_DispStringAt("K5",250,170);
          osDelay(10);
        } 
        if ( i % 10 == 6) {
          GUI_ClearRect(250,170,310,190);
          GUI_SetColor(GUI_LIGHTRED);
          GUI_SetFont(GUI_FONT_24B_1);
          GUI_DispStringAt("K5>",250,170);
          osDelay(10);
        }
        if ( i % 10 == 9) {
          GUI_ClearRect(250,170,310,190);
          GUI_SetColor(GUI_LIGHTRED);
          GUI_SetFont(GUI_FONT_24B_1);
          GUI_DispStringAt("K5>>",250,170);
          osDelay(10);
        }
        
      }

      GUI_SetColor(GUI_ORANGE);
      GUI_FillRect(40,200,4*i+40,220);
      GUI_DrawRect(40,200,280,220);

      if ( i == 60 ) 
      {
         GUI_SetColor(GUI_BROWN);
         GUI_SetFont(GUI_FONT_20_ASCII);
         GUI_DispStringAt("100%", 132, 204);
         osDelay(500);
      }
      osDelay(20);
    }

    if ( !Book.button ) 
    {

        GUI_Clear();
        GUI_SetBkColor(GUI_GRAY_D0);
        GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
        GUI_SetFont(&GUI_FontHZ_YouYuan_24);
        GUI_SetColor(GUI_RED);
        
        GUI_DrawBitmap(&bmhdu, 0, 0);
        GUI_DispStringAt("∑¿ª∑¿µ¡º‡≤‚∆˜", 76, 96);
        if (!Book.button) 
          osDelay(1000);
        GUI_Clear();
        GUI_DispStringAt("21032311", 106, 96);
        GUI_DispStringAt("≥¬Ï≈»Û", 114, 126);
        if (!Book.button)
        {
          osDelay(2000);
          GUI_Clear();
          GUI_SetBkColor(GUI_GRAY_D0);
          GUI_SetColor(GUI_DARKCYAN);
          GUI_SetFont(&GUI_FontHZ_Zhongyuan_Hz_24);
          GUI_DispStringAt("µÁ◊”…Ëº∆◊€∫œ µº˘", 60, 30);
          GUI_DrawBitmap(&bmcxr, 120, 70);
          osDelay(1000);
        } 

    }
    Beep(7,100);
}

void Beep(uint8_t tune, uint16_t time)
{
    static uint16_t TAB[] = {494, 523, 588, 660, 698, 784, 880, 988};

    if (tune >=1  && tune <= 7) {
      int pre = 1000000 / TAB[tune];
      __HAL_TIM_SET_AUTORELOAD(&htim3,pre);
      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pre/2);
      
      beeptick = osKernelGetTickCount() + time;
      HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    }
}

void Beep_mpu(uint8_t tune, uint16_t time)
{
    static uint16_t TAB[] = {262, 294, 330, 349, 392, 440, 494, 523};
    if (tune >=1  && tune <= 7) {
      int pre = 1000000 / TAB[tune];
      __HAL_TIM_SET_AUTORELOAD(&htim3,pre);
      __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pre/2);
      
      beeptick = osKernelGetTickCount() + time;
      HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    }
}

void BeepDone(void)
{
   if(beeptick > 0 && osKernelGetTickCount() >= beeptick) {
    beeptick = 0;
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
   }
}

void UpdateDisplayOnButtonPress(void)
{
    gtemp.update = 1;
    Book_Pageturn(Book.cur, gtemp, mpu);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
        case K5_Pin:
          if (HAL_GPIO_ReadPin(K5_GPIO_Port, K5_Pin) == SET && Book.Priority != WIFI_ESP && Book.Priority != PARA_SET )
          {
              Book.cur  = MENU[Book.cur].enter;
              Book.last = 0;
              Book.button = 1;
              UpdateDisplayOnButtonPress(); 
          }
        break;
        default:
        break;
    }
}
/* USER CODE END Application */

