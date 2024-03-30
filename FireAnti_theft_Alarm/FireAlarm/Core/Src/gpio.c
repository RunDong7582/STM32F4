/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include <cmsis_os2.h>
#include "tim.h"

uint16_t beep_time = 0;
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

  /**/
  LL_GPIO_ResetOutputPin(GPIOB, LCD_BL_Pin|BEEP_Pin);

  /**/
  LL_GPIO_ResetOutputPin(LCD_RST_GPIO_Port, LCD_RST_Pin);

  /**/
  LL_GPIO_ResetOutputPin(DATA_GPIO_Port, DATA_Pin);

  /**/
  GPIO_InitStruct.Pin = K2_Pin|K3_Pin|K4_Pin|K1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = K5_Pin|K6_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LCD_BL_Pin|BEEP_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LCD_RST_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LCD_RST_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = DATA_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DATA_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
uint16_t ScanKey(void)
{
  uint16_t key =0;
	if (LL_GPIO_IsInputPinSet(GPIOE ,LL_GPIO_PIN_1) == RESET) key |= K1_Pin;
	if (LL_GPIO_IsInputPinSet(GPIOE ,LL_GPIO_PIN_2) == RESET) key |= K2_Pin;
	if (LL_GPIO_IsInputPinSet(GPIOE ,LL_GPIO_PIN_3) == RESET) key |= K3_Pin;
	if (LL_GPIO_IsInputPinSet(GPIOE ,LL_GPIO_PIN_4) == RESET) key |= K4_Pin;
  if (LL_GPIO_IsInputPinSet(GPIOE ,LL_GPIO_PIN_5) == SET)   key |= K5_Pin;
	if (LL_GPIO_IsInputPinSet(GPIOE ,LL_GPIO_PIN_6) == SET)   key |= K6_Pin;

	if(key > 0)
	{
	  HAL_Delay(20);
	  uint16_t  key2 = 0;
	  if (LL_GPIO_IsInputPinSet(GPIOE ,LL_GPIO_PIN_1) == RESET) key2 |= K1_Pin;
    if (LL_GPIO_IsInputPinSet(GPIOE ,LL_GPIO_PIN_2) == RESET) key2 |= K2_Pin;
    if (LL_GPIO_IsInputPinSet(GPIOE ,LL_GPIO_PIN_3) == RESET) key2 |= K3_Pin;
    if (LL_GPIO_IsInputPinSet(GPIOE ,LL_GPIO_PIN_4) == RESET) key2 |= K4_Pin;
    if (LL_GPIO_IsInputPinSet(GPIOE ,LL_GPIO_PIN_5) == SET)   key2 |= K5_Pin;
    if (LL_GPIO_IsInputPinSet(GPIOE ,LL_GPIO_PIN_6) == SET)   key2 |= K6_Pin;
    if(key == key2)
        return key;
    else 
        return 0;
  }
	return 0;
}

void Beep(uint8_t tune, uint16_t time)
{
  const float tab[8] = {0, 261.6, 293.6, 329.6, 349.2, 392.0, 440.0, 493.9};
	int key = 0;
	if(tune > 0 && tune <= 21)
	{
	  key = (tune - 1) / 7; //0 1 2
    while(tune > 7)
      tune -= 7;		
	}
	
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
	if(tune > 0)//有效音调
	{
    float arr =(1000000 / tab[tune])/2;
    while (key-- > 0)
			 arr /= 2;
		   arr -= 1;
    __HAL_TIM_SET_AUTORELOAD(&htim3, (uint16_t)arr); //重新调整频率
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1,(uint16_t)arr/2);
		HAL_TIM_PWM_Start(&htim3 , TIM_CHANNEL_1);
		beep_time = time;
  }
}
/* USER CODE END 2 */
