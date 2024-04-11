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
#include "printf.h"
#include "string.h"
#include "../../API/Menu.h"
#include "../inc/gpio.h"
#include "../inc/tim.h"
#include "../inc/usart.h"

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
#define TH 125.0
#define TW 28.0
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

Temperature gtemp = {
    0.0f, 
    0.0f,
       0,
       0,
       0,
       0
};

MPUpacket mpu = {
       1,         // ax
       0,         // ay
      -1,         // az
       1,         // gx
       0,         // gy
      -1,         // gz
       0,         // ok
       0,         // theft-warn
       1,         // update
       0          // Plot counter
};

Page_t Book = {
        1, 
      127, 
        0,
TEMP_FIRST,
TEMP_CURVE,
        1,
};

uint32_t beeptick = 0;


/* USER CODE END Variables */
/* Definitions for MainTask */
osThreadId_t MainTaskHandle;
const osThreadAttr_t MainTask_attributes = {
  .name = "MainTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for KeyTask */
osThreadId_t KeyTaskHandle;
const osThreadAttr_t KeyTask_attributes = {
  .name = "KeyTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UartTask */
osThreadId_t UartTaskHandle;
const osThreadAttr_t UartTask_attributes = {
  .name = "UartTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for GUITask */
osThreadId_t GUITaskHandle;
const osThreadAttr_t GUITask_attributes = {
  .name = "GUITask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void GUI_Start();
void Beep(uint8_t tune, uint16_t time);
void BeepDone(void);
/* USER CODE END FunctionPrototypes */

void DefaultMainTask(void *argument);
void StartKeyTask(void *argument);
void StartUartTask(void *argument);
void StartGUITask(void *argument);

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
  /* creation of MainTask */
  MainTaskHandle = osThreadNew(DefaultMainTask, NULL, &MainTask_attributes);

  /* creation of KeyTask */
  KeyTaskHandle = osThreadNew(StartKeyTask, NULL, &KeyTask_attributes);

  /* creation of UartTask */
  UartTaskHandle = osThreadNew(StartUartTask, NULL, &UartTask_attributes);

  /* creation of GUITask */
  GUITaskHandle = osThreadNew(StartGUITask, NULL, &GUITask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_DefaultMainTask */
/**
  * @brief  Function implementing the MainTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_DefaultMainTask */
void DefaultMainTask(void *argument)
{
  /* USER CODE BEGIN DefaultMainTask */
  /* Infinite loop */
  ds18b20_init();
  for(;;)
  {
      if ( Book.Priority == TEMP_FIRST ) 
      {

          osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);

          osDelay(1000);
          gtemp.new  = ds18b20_read();

      }
      else if ( Book.Priority == TEMP_CURVE ) 
      {
          osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
          
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

              if ( gtemp.new >= TW ) { gtemp.warn = 1;}
          }
      }
  }
  /* USER CODE END DefaultMainTask */
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
		uint16_t key = ScanKey();

      if ( key > 0 )
      {
          switch ( key )
          {
              case K1_Pin:
                Book.cur = MENU[Book.cur].up;

                Book_Priorswitch (Book.cur);     
                osDelay(500);
              break;

              case K4_Pin:
                Book.cur = MENU[Book.cur].down;

                Book_Priorswitch (Book.cur);    
                osDelay(500);
              break;

              case K2_Pin:
                Book.cur = MENU[Book.cur].left;  

                Book_Priorswitch (Book.cur);  
                osDelay(500);
              break;

              case K3_Pin:    
                Book.cur = MENU[Book.cur].right; 

                Book_Priorswitch (Book.cur);   
                osDelay(500);
              break;

              case K5_Pin:
                Book.cur = MENU[Book.cur].enter;

                Book_Priorswitch (Book.cur);   
                osDelay(500);
              break;

              case K6_Pin:
                Book.cur = MENU[Book.cur].back;

                Book_Priorswitch (Book.cur);   
                osDelay(500);
              break;

              default:
              break;
          }
      }

    osDelay(1);
  }
  /* USER CODE END StartKeyTask */
}

/* USER CODE BEGIN Header_StartUartTask */
/**
* @brief Function implementing the UartTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUartTask */
void StartUartTask(void *argument)
{
  /* USER CODE BEGIN StartUartTask */
	
  mpu.ok = MPU_init();

  if ( mpu.ok ) 
  {
      GUI_ClearRect(72,20,252,50);
      GUI_SetFont(&GUI_FontHZ_Zhongyuan_HZ_24);
      GUI_SetColor(GUI_LIGHTRED);
      GUI_DispStringAt("MPU³õÊ¼»¯³É¹¦!",72,20);
      osDelay(700);
      GUI_ClearRect(72,20,252,50);
  }

  /* Infinite loop */
  for(;;)
  {    
      if ( mpu.ok && Book.Priority == MPU_FIRST ) 
      {
          osThreadFlagsWait(0x0002, osFlagsWaitAny, osWaitForever);

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

      else if ( mpu.ok && Book.Priority > TEMP_CURVE )
      {
          
          osThreadFlagsWait(0x0002, osFlagsWaitAny, osWaitForever);

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
      }
      if ( gx * gx + gy * gy + gz * gz > 2500 ) 
      {
        mpu.warn = 1;
        // Beep(1,100);
      } else {
        mpu.warn = 0;
        // BeepDone();
      }
      osDelay(50);
  }
  /* USER CODE END StartUartTask */
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

  printf("Hello STM32F407!\n");

  osThreadFlagsSet(MainTaskHandle , 0x0001);
  osDelay(2U);

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
                    osThreadFlagsSet(MainTaskHandle , 0x0001);

                    osDelay(2U);
                  }

              break;

              case  MPU_FIRST:

                  if ( mpu.update ) 
                  {
                    Book_Pageturn ( Book.cur, gtemp, mpu );
                    mpu.update = 0;
                  }
                  if ( !mpu.update ) 
                  {
                    osThreadFlagsSet ( UartTaskHandle , 0x0002 );

                    osDelay(100U);
                  }

              break;

              case  TEMP_CURVE:

                  osThreadFlagsSet ( MainTaskHandle , 0x0001 );
                  osDelay(2U);
                  Book_Pageturn ( Book.cur, gtemp, mpu );

              break;

              case   MPU_YAW:

                  osThreadFlagsSet ( UartTaskHandle , 0x0002 );
                  osDelay(2U);
                  Book_Pageturn ( Book.cur, gtemp, mpu );

              break;

              case   MPU_ROLL:

                  osThreadFlagsSet ( UartTaskHandle , 0x0002 );
                  osDelay(2U);
                  Book_Pageturn ( Book.cur, gtemp, mpu );

              break;

              case   MPU_PITCH:

                  osThreadFlagsSet ( UartTaskHandle , 0x0002 );
                  osDelay(2U);
                  Book_Pageturn ( Book.cur, gtemp, mpu );

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

    if (!Book.button) 
    {

        GUI_Clear();
        GUI_SetBkColor(GUI_GRAY_D0);
        GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
        GUI_SetFont(&GUI_FontHZ_YouYuan_24);
        GUI_SetColor(GUI_RED);
        
        GUI_DrawBitmap(&bmhdu, 0, 0);
        GUI_DispStringAt("·À»ð·ÀµÁ¼à²âÆ÷", 76, 96);
        if (!Book.button) 
          osDelay(1000);
        GUI_Clear();
        GUI_DispStringAt("21032311", 106, 96);
        GUI_DispStringAt("³ÂìÅÈó", 114, 126);
        if (!Book.button) 
          osDelay(2000);

    }
    // Beep(7,100);
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
          if (HAL_GPIO_ReadPin(K5_GPIO_Port, K5_Pin) == SET)
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

