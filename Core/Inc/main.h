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
#define Key1_Pin GPIO_PIN_0
#define Key1_GPIO_Port GPIOC
#define Key2_Pin GPIO_PIN_1
#define Key2_GPIO_Port GPIOC
#define Key3_Pin GPIO_PIN_2
#define Key3_GPIO_Port GPIOC
#define Encoder_TIM5_CH1_Pin GPIO_PIN_0
#define Encoder_TIM5_CH1_GPIO_Port GPIOA
#define Encoder_TIM5_CH2_Pin GPIO_PIN_1
#define Encoder_TIM5_CH2_GPIO_Port GPIOA
#define N10_USART2_TX_Pin GPIO_PIN_2
#define N10_USART2_TX_GPIO_Port GPIOA
#define N10_USART2_RX_Pin GPIO_PIN_3
#define N10_USART2_RX_GPIO_Port GPIOA
#define Encoder_TIM2_CH1_Pin GPIO_PIN_5
#define Encoder_TIM2_CH1_GPIO_Port GPIOA
#define Encoder_TIM3_CH1_Pin GPIO_PIN_6
#define Encoder_TIM3_CH1_GPIO_Port GPIOA
#define Encoder_TIM3_CH2_Pin GPIO_PIN_7
#define Encoder_TIM3_CH2_GPIO_Port GPIOA
#define M1__Pin GPIO_PIN_4
#define M1__GPIO_Port GPIOC
#define M1_C5_Pin GPIO_PIN_5
#define M1_C5_GPIO_Port GPIOC
#define M2__Pin GPIO_PIN_0
#define M2__GPIO_Port GPIOB
#define M2_B1_Pin GPIO_PIN_1
#define M2_B1_GPIO_Port GPIOB
#define M3__Pin GPIO_PIN_7
#define M3__GPIO_Port GPIOE
#define M3_E8_Pin GPIO_PIN_8
#define M3_E8_GPIO_Port GPIOE
#define Motor1_TIM1_CH1_Pin GPIO_PIN_9
#define Motor1_TIM1_CH1_GPIO_Port GPIOE
#define Motor2_TIM1_CH2_Pin GPIO_PIN_11
#define Motor2_TIM1_CH2_GPIO_Port GPIOE
#define M4__Pin GPIO_PIN_12
#define M4__GPIO_Port GPIOE
#define Motor3_TIM1_CH3_Pin GPIO_PIN_13
#define Motor3_TIM1_CH3_GPIO_Port GPIOE
#define M4_E15_Pin GPIO_PIN_15
#define M4_E15_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_11
#define LED2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_12
#define LED3_GPIO_Port GPIOB
#define BUFFER_Pin GPIO_PIN_13
#define BUFFER_GPIO_Port GPIOB
#define BlueTooth_USART3_TX_Pin GPIO_PIN_8
#define BlueTooth_USART3_TX_GPIO_Port GPIOD
#define BlueTooth_USART3_RX_Pin GPIO_PIN_9
#define BlueTooth_USART3_RX_GPIO_Port GPIOD
#define Encoder_TIM4_CH1_Pin GPIO_PIN_12
#define Encoder_TIM4_CH1_GPIO_Port GPIOD
#define Encoder_TIM4_CH2_Pin GPIO_PIN_13
#define Encoder_TIM4_CH2_GPIO_Port GPIOD
#define Steer_USART6_TX_Pin GPIO_PIN_6
#define Steer_USART6_TX_GPIO_Port GPIOC
#define Steer_USART6_RX_Pin GPIO_PIN_7
#define Steer_USART6_RX_GPIO_Port GPIOC
#define HWT101_USART1_TX_Pin GPIO_PIN_9
#define HWT101_USART1_TX_GPIO_Port GPIOA
#define HWT101_USART1_RX_Pin GPIO_PIN_10
#define HWT101_USART1_RX_GPIO_Port GPIOA
#define K230_UART5_TX_Pin GPIO_PIN_12
#define K230_UART5_TX_GPIO_Port GPIOC
#define K230_UART_RX_Pin GPIO_PIN_2
#define K230_UART_RX_GPIO_Port GPIOD
#define Encoder_TIM2_CH2_Pin GPIO_PIN_3
#define Encoder_TIM2_CH2_GPIO_Port GPIOB
#define I2C1_SCL_Pin GPIO_PIN_6
#define I2C1_SCL_GPIO_Port GPIOB
#define I2C1_SDA_Pin GPIO_PIN_7
#define I2C1_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
