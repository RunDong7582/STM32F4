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
// #include "stdio.h"
#include "string.h"
#include "../inc/gpio.h"
#include "DS18B20.h"
#include "MPU6050.h"
#include "../../Drivers/STemWin/inc/GUI.h"
#include "touch.h"
#include <cmsis_os2.h>
#include "../../API/Menu.h"
#include "stm32f4xx_ll_gpio.h"
#include "tim.h"
#include "usart.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define uchar unsigned char
typedef struct {
    uchar cur;
    uchar last;
}Page_t;

typedef struct {
    float new;
    float old;
}Temperature;

// typedef struct {
//     __IO short ax;
//     __IO short ay;
//     __IO short az;
//     __IO short gx;
//     __IO short gy;
//     __IO short gz;
// }MPUpacket;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
int mpuok = 0;
uint16_t button = 0;
uint16_t key = 0;
Temperature gtemp = {0.0f, 0.0f};
MPUpacket mpu = {1,0,-1,1,0,-1};
Page_t Book = {1 , 127};

uint32_t beeptick = 0;
extern const GUI_FONT GUI_FontHZ_YouYuan_24;
extern const GUI_FONT GUI_FontHZ_KaiTi_32;
extern const GUI_FONT GUI_FontHZ_KaiTi_20;
extern const GUI_FONT GUI_FontHZ_KaiTi_12;
extern const GUI_FONT GUI_FontHZ_Zhongyuan_Hz_24;
extern GUI_CONST_STORAGE GUI_BITMAP bmhdu;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarmbw;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm30;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm60;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm82;

extern __IO short ax, ay, az, gx, gy, gz;	// ????
/* USER CODE END Variables */
/* Definitions for MainTask */
osThreadId_t MainTaskHandle;
const osThreadAttr_t MainTask_attributes = {
  .name = "MainTask",
  .stack_size = 512 * 4,
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
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void GUI_Start();
// static void (*current_display_page)();
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
	mpuok = MPU_init();
	int cnt = 0;
	while (!mpuok && cnt < 3)
	{
		osDelay(500);
		mpuok = MPU_init();
		++cnt;
	}
  for(;;)
  {
    osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
    osDelay(1000);
    gtemp.new  = ds18b20_read();
    osDelay(500);
    if (mpuok) {
      MPU_getdata();
      mpu.ax = ax;
      mpu.ay = ay;
      mpu.az = az;
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
		key = ScanKey();
    if(key > 0)
    {
      switch (key)
      {
        case K1_Pin:
          Book.cur = MENU[Book.cur].up;     
          osDelay(500);
        break;
        case K4_Pin:
          Book.cur = MENU[Book.cur].down;   
          osDelay(500);
        break;
        case K2_Pin:
          Book.cur = MENU[Book.cur].left;   
          osDelay(500);
        break;
        case K3_Pin:    
          Book.cur = MENU[Book.cur].right;   
          osDelay(500);
        break;
        case K5_Pin:
          Book.cur = MENU[Book.cur].enter;  
          osDelay(500);
        break;
        case K6_Pin:
          Book.cur = MENU[Book.cur].back;  
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
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
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
  GUI_Init();
  GUI_Start();
  osThreadFlagsSet(MainTaskHandle , 0x0001);
  osDelay(2U);
  for(;;)
  {
      if ( Book.last != Book.cur ) {
          Book_Pageturn(Book.cur,gtemp.new,0,mpu);
          Book.last = Book.cur;
      }
      else {
        if ( gtemp.new != gtemp.old ) {
          Book_Pageturn(Book.cur,gtemp.new,1,mpu);
          gtemp.old = gtemp.new;
          button = 0;
        } else {
          button = 0;
          osThreadFlagsSet(MainTaskHandle , 0x0001);
          osDelay(2U);
        }
      }
      if (Book.cur == 0) {Book.cur = 1;}
      osDelay(100);

    BeepDone();
    osDelay(1);
  }
  /* USER CODE END StartGUITask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
static void GUI_Start()
{    
	  GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
    GUI_SetBkColor(GUI_BLACK);
    for(int i = 0; i < 61; i++) {
      if(button) {GUI_Clear();GUI_SetBkColor(GUI_GRAY_D0);GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());break;}
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
      GUI_SetColor(GUI_ORANGE);
      GUI_FillRect(40,200,4*i+40,220);
      GUI_DrawRect(40,200,280,220);
      if( i == 60 ){
         GUI_SetColor(GUI_BROWN);
         GUI_SetFont(GUI_FONT_20_ASCII);
         GUI_DispStringAt("100%", 132, 204);
         osDelay(500);
      }
      osDelay(20);
    }
    if (!button) {
      GUI_Clear();
      GUI_SetBkColor(GUI_GRAY_D0);
      GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
      GUI_SetFont(&GUI_FontHZ_YouYuan_24);
      GUI_SetColor(GUI_RED);
      
      GUI_DrawBitmap(&bmhdu, 0, 0);
      GUI_DispStringAt("·À»ð·ÀµÁ¼à²âÆ÷", 76, 96);
      osDelay(1000);
      GUI_Clear();
      GUI_DispStringAt("21032311", 106, 96);
      GUI_DispStringAt("³ÂìÅÈó", 114, 126);
      osDelay(2000);
    }
    //Beep(7,100);
}

void Beep(uint8_t tune, uint16_t time)
{
    static uint16_t TAB[] = {494, 523, 588, 660, 698, 784, 880, 988};
    HAL_TIM_Base_Start(&htim3);

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
    Book_Pageturn(Book.cur,gtemp.new,1,mpu);
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
              button = 1;
              UpdateDisplayOnButtonPress(); 
          }
        break;
        default:
        break;
    }
}
/* USER CODE END Application */

