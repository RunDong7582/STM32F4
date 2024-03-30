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
#include "stdio.h"
#include "string.h"
#include "../inc/gpio.h"
#include "DS18B20.h"
#include "../../Drivers/STemWin/inc/GUI.h"
#include "touch.h"
#include <cmsis_os2.h>
#include "../../API/Menu.h"
#include "stm32f4xx_ll_gpio.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
float g_tmp = 0;
uint16_t key = 0;
unsigned char func_index = 0;
unsigned char last_index = 127;
extern const GUI_FONT GUI_FontHZ_YouYuan_24;
extern const GUI_FONT GUI_FontHZ_KaiTi_32;
extern const GUI_FONT GUI_FontHZ_KaiTi_20;
extern const GUI_FONT GUI_FontHZ_KaiTi_12;
extern GUI_CONST_STORAGE GUI_BITMAP bmhdu;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarmbw;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm30;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm60;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm82;

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
static void (*current_operation_index)();
uint16_t KEY_CHECK(uint8_t ch);
// static void GUI_Menu();
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
  g_tmp = ds18b20_read();
  for(;;)
  {
    osDelay(1);
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
          func_index = table[func_index].up;      //向上翻
        break;
        case K2_Pin:
          func_index = table[func_index].down;    //向下翻
        break;
        case K3_Pin:
          func_index = table[func_index].enter;   //确认
        break;
        case K4_Pin:
          func_index = last_index;                //返回上一页面
        break;
        case K5_Pin:
          GUI_Clear();
        break;
        case K6_Pin:
          GUI_Clear();
        break;
        default:
        break;
      }
    }

		// osThreadFlagsSet(StartGUITask, key);
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
  // GUI_Start();
  // GUI_Menu();
  for(;;)
  {
    if (func_index != last_index)
		{
			current_operation_index = table[func_index].current_operation;
      GUI_Clear();
			(*current_operation_index)();//执行当前操作函数
			
			last_index = func_index;
		}

  	// uint32_t gGui = osThreadFlagsWait( K1_Pin | K2_Pin  | K3_Pin | K4_Pin | K5_Pin | K6_Pin ,osFlagsWaitAny,10);
    // switch (gGui)
    // {
    //     case K1_Pin:
    //     break;
    //     case K2_Pin:
    //     break;
    //     case K3_Pin:
    //     break;
    //     case K4_Pin:
    //     break;
    //     case K5_Pin:
    //     break;
    //     case K6_Pin:
    //       GUI_Start();
    //     break;
    //     default:
    //     break;
    //}

    osDelay(1);
  }
  /* USER CODE END StartGUITask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
static void GUI_Start()
{    
    GUI_SetBkColor(GUI_BLACK);
    for(int i = 0; i < 61; i++) {
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
      GUI_SetColor(GUI_DARKRED);
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
    GUI_Clear();
    GUI_SetBkColor(GUI_GRAY_D0);
    GUI_SetFont(&GUI_FontHZ_YouYuan_24);
    GUI_SetColor(GUI_RED);
    
    GUI_DrawBitmap(&bmhdu, 0, 0);
    GUI_DispStringAt("防火防盗监测器", 76, 96);
    osDelay(1000);
    GUI_Clear();
    GUI_DispStringAt("21032311", 106, 96);
    GUI_DispStringAt("陈炫润", 114, 126);
    osDelay(2000);
}

// static void GUI_Menu()
// {
//     GUI_SetBkColor(GUI_GRAY_D0);
//     GUI_SetColor(GUI_BROWN);
//     GUI_SetFont(&GUI_FontHZ_KaiTi_20);
//     current_operation_index = table[func_index].current_operation;
//     GUI_Clear();
//     (*current_operation_index)();
// }
/* USER CODE END Application */

