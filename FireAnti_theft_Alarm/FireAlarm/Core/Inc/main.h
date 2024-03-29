/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include "stm32f4xx_ll_i2c.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define K2_Pin LL_GPIO_PIN_2
#define K2_GPIO_Port GPIOE
#define K3_Pin LL_GPIO_PIN_3
#define K3_GPIO_Port GPIOE
#define K4_Pin LL_GPIO_PIN_4
#define K4_GPIO_Port GPIOE
#define K5_Pin LL_GPIO_PIN_5
#define K5_GPIO_Port GPIOE
#define K6_Pin LL_GPIO_PIN_6
#define K6_GPIO_Port GPIOE
#define LCD_BL_Pin LL_GPIO_PIN_10
#define LCD_BL_GPIO_Port GPIOB
#define T_CS_Pin LL_GPIO_PIN_12
#define T_CS_GPIO_Port GPIOB
#define LCD_RST_Pin LL_GPIO_PIN_13
#define LCD_RST_GPIO_Port GPIOD
#define BEEP_Pin LL_GPIO_PIN_4
#define BEEP_GPIO_Port GPIOB
#define K1_Pin LL_GPIO_PIN_1
#define K1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */