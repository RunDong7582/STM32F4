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
#define K2_Pin GPIO_PIN_2
#define K2_GPIO_Port GPIOE
#define K3_Pin GPIO_PIN_3
#define K3_GPIO_Port GPIOE
#define K4_Pin GPIO_PIN_4
#define K4_GPIO_Port GPIOE
#define K5_Pin GPIO_PIN_5
#define K5_GPIO_Port GPIOE
#define K5_EXTI_IRQn EXTI9_5_IRQn
#define K6_Pin GPIO_PIN_6
#define K6_GPIO_Port GPIOE
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOC
#define LCD_BL_Pin GPIO_PIN_10
#define LCD_BL_GPIO_Port GPIOB
#define T_CS_Pin GPIO_PIN_12
#define T_CS_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_13
#define LCD_RST_GPIO_Port GPIOD
#define I2C_SCL_Pin GPIO_PIN_6
#define I2C_SCL_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_7
#define I2C_SDA_GPIO_Port GPIOB
#define DATA_Pin GPIO_PIN_0
#define DATA_GPIO_Port GPIOE
#define K1_Pin GPIO_PIN_1
#define K1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
#define KSUM ((K1_Pin) | (K2_Pin) | (K3_Pin) | (K4_Pin))
#define K_IDIE 0x0080
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
