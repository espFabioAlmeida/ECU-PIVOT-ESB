/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#define RELE1_Pin GPIO_PIN_2
#define RELE1_GPIO_Port GPIOE
#define RELE2_Pin GPIO_PIN_3
#define RELE2_GPIO_Port GPIOE
#define RELE3_Pin GPIO_PIN_4
#define RELE3_GPIO_Port GPIOE
#define RELE4_Pin GPIO_PIN_5
#define RELE4_GPIO_Port GPIOE
#define SW_DOWN_Pin GPIO_PIN_4
#define SW_DOWN_GPIO_Port GPIOC
#define SW_OK_Pin GPIO_PIN_5
#define SW_OK_GPIO_Port GPIOC
#define LED_COM_Pin GPIO_PIN_2
#define LED_COM_GPIO_Port GPIOB
#define LED_CPU_Pin GPIO_PIN_7
#define LED_CPU_GPIO_Port GPIOE
#define SW_UP_Pin GPIO_PIN_8
#define SW_UP_GPIO_Port GPIOE
#define RELE5_Pin GPIO_PIN_9
#define RELE5_GPIO_Port GPIOE
#define RELE6_Pin GPIO_PIN_10
#define RELE6_GPIO_Port GPIOE
#define RELE7_Pin GPIO_PIN_11
#define RELE7_GPIO_Port GPIOE
#define RELE8_Pin GPIO_PIN_12
#define RELE8_GPIO_Port GPIOE
#define RELE10_Pin GPIO_PIN_13
#define RELE10_GPIO_Port GPIOE
#define RELE9_Pin GPIO_PIN_14
#define RELE9_GPIO_Port GPIOE
#define SW_LEFT_Pin GPIO_PIN_15
#define SW_LEFT_GPIO_Port GPIOE
#define LCD_RS_Pin GPIO_PIN_12
#define LCD_RS_GPIO_Port GPIOB
#define LCD_ENABLE_Pin GPIO_PIN_13
#define LCD_ENABLE_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_14
#define BUZZER_GPIO_Port GPIOB
#define LCD_BKL_Pin GPIO_PIN_15
#define LCD_BKL_GPIO_Port GPIOB
#define LCD_D7_Pin GPIO_PIN_8
#define LCD_D7_GPIO_Port GPIOD
#define LCD_D4_Pin GPIO_PIN_9
#define LCD_D4_GPIO_Port GPIOD
#define LCD_D5_Pin GPIO_PIN_10
#define LCD_D5_GPIO_Port GPIOD
#define LCD_D6_Pin GPIO_PIN_11
#define LCD_D6_GPIO_Port GPIOD
#define IN3_Pin GPIO_PIN_12
#define IN3_GPIO_Port GPIOD
#define IN4_Pin GPIO_PIN_13
#define IN4_GPIO_Port GPIOD
#define IN5_Pin GPIO_PIN_14
#define IN5_GPIO_Port GPIOD
#define IN6_Pin GPIO_PIN_15
#define IN6_GPIO_Port GPIOD
#define IN1_Pin GPIO_PIN_10
#define IN1_GPIO_Port GPIOC
#define IN2_Pin GPIO_PIN_11
#define IN2_GPIO_Port GPIOC
#define IOT_PON_TRIG_Pin GPIO_PIN_12
#define IOT_PON_TRIG_GPIO_Port GPIOC
#define IOT_PSM_IND_Pin GPIO_PIN_0
#define IOT_PSM_IND_GPIO_Port GPIOD
#define SOQUETE_OUT1_Pin GPIO_PIN_1
#define SOQUETE_OUT1_GPIO_Port GPIOD
#define LORA_AUX_Pin GPIO_PIN_2
#define LORA_AUX_GPIO_Port GPIOD
#define LORA_M1_Pin GPIO_PIN_3
#define LORA_M1_GPIO_Port GPIOD
#define LORA_M0_Pin GPIO_PIN_4
#define LORA_M0_GPIO_Port GPIOD
#define SOQUETE_OUT2_Pin GPIO_PIN_4
#define SOQUETE_OUT2_GPIO_Port GPIOB
#define SW_RIGHT_Pin GPIO_PIN_5
#define SW_RIGHT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
