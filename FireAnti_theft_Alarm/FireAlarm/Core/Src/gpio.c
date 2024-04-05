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

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_BL_Pin|I2C_SCL_Pin|I2C_SDA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DATA_GPIO_Port, DATA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PEPin PEPin PEPin PEPin */
  GPIO_InitStruct.Pin = K2_Pin|K3_Pin|K4_Pin|K1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = K5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(K5_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = K6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(K6_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = LCD_BL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_BL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = LCD_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = I2C_SCL_Pin|I2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = DATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DATA_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 2 */

uint16_t ScanKey(void) {
		uint16_t key = 0;
		if (HAL_GPIO_ReadPin(K1_GPIO_Port,K1_Pin) == GPIO_PIN_RESET)
			  key |= K1_Pin;
		if (HAL_GPIO_ReadPin(K2_GPIO_Port,K2_Pin) == GPIO_PIN_RESET)
			  key |= K2_Pin;
		if (HAL_GPIO_ReadPin(K3_GPIO_Port,K3_Pin) == GPIO_PIN_RESET)
			  key |= K3_Pin;
		if (HAL_GPIO_ReadPin(K4_GPIO_Port,K4_Pin) == GPIO_PIN_RESET)
			  key |= K4_Pin;
		if (HAL_GPIO_ReadPin(K5_GPIO_Port,K5_Pin) == GPIO_PIN_SET)
			  key |= K5_Pin;
		if (HAL_GPIO_ReadPin(K6_GPIO_Port,K6_Pin) == GPIO_PIN_SET)
			  key |= K6_Pin;
		if (key > 0){
			  osDelay(10);
			  uint16_t key2 = 0;
		    if (HAL_GPIO_ReadPin(K1_GPIO_Port,K1_Pin) == GPIO_PIN_RESET)
			      key2 |= K1_Pin;
		    if (HAL_GPIO_ReadPin(K2_GPIO_Port,K2_Pin) == GPIO_PIN_RESET)
			      key2 |= K2_Pin;
		    if (HAL_GPIO_ReadPin(K3_GPIO_Port,K3_Pin) == GPIO_PIN_RESET)
			      key2 |= K3_Pin;
		    if (HAL_GPIO_ReadPin(K4_GPIO_Port,K4_Pin) == GPIO_PIN_RESET)
			      key2 |= K4_Pin;
		    if (HAL_GPIO_ReadPin(K5_GPIO_Port,K5_Pin) == GPIO_PIN_SET)
			      key2 |= K5_Pin;
		    if (HAL_GPIO_ReadPin(K6_GPIO_Port,K6_Pin) == GPIO_PIN_SET)
			      key2 |= K6_Pin;
				if (key == key2)
					  return key;
				else
					  return 0;
		}
		return 0;
	}

/* USER CODE END 2 */
