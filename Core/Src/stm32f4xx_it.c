/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MySys.h"
#include "Imu.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim12;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart5_rx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim6;

/* USER CODE BEGIN EV */

extern MyUartGroup uartGroup;
extern osMessageQueueId_t Queue_ImuHandle;
extern osMessageQueueId_t Queue_CameraHandle;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

  /* USER CODE END DMA1_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart5_rx);
  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
void DMA1_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream2_IRQn 0 */

  /* USER CODE END DMA1_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart4_rx);
  /* USER CODE BEGIN DMA1_Stream2_IRQn 1 */

  /* USER CODE END DMA1_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream5 global interrupt.
  */
void DMA1_Stream5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream5_IRQn 0 */

  /* USER CODE END DMA1_Stream5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
  /* USER CODE BEGIN DMA1_Stream5_IRQn 1 */

  /* USER CODE END DMA1_Stream5_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        HAL_UART_DMAStop(&huart1);
        uint16_t recv_len = uartGroup.imu.buff - __HAL_DMA_GET_COUNTER(huart1.hdmarx);
        register uint16_t i = 0;
        while (recv_len - i >= uartGroup.imu.len) {
            if (uartGroup.imu.Receive_data[i] == 0x55 ) {
                memcpy(uartGroup.imu.prcData, &uartGroup.imu.Receive_data[i], uartGroup.imu.len);          
                i += uartGroup.imu.len;
            } else i++;
        }
        memset(uartGroup.imu.Receive_data, 0, uartGroup.imu.buff);
        HAL_UART_Receive_DMA(&huart1, uartGroup.imu.Receive_data, uartGroup.imu.buff);
    }
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
  if(UART2MODE==0)
  {
  if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE)) {
      __HAL_UART_CLEAR_IDLEFLAG(&huart2);
      HAL_UART_DMAStop(&huart2);
      uint16_t recv_len = uartGroup.scan.buff - __HAL_DMA_GET_COUNTER(huart2.hdmarx);
      if (recv_len >= uartGroup.scan.len) {
          for (uint16_t i = 0; i <= recv_len - uartGroup.scan.len; i++) {
              if (uartGroup.scan.Receive_data[i] == 0xA5 && uartGroup.scan.Receive_data[i+1] == 0x5A) {
                  memcpy(uartGroup.scan.prcData, &uartGroup.scan.Receive_data[i], uartGroup.scan.len);
                  break;
              }
          }
      }
      HAL_UART_Receive_DMA(&huart2, uartGroup.scan.Receive_data, uartGroup.scan.buff);
    }
  }
  if(UART2MODE==1)
{
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart2);
        HAL_UART_DMAStop(&huart2);
        uint16_t recv_len = uartGroup.STP.buff - __HAL_DMA_GET_COUNTER(huart2.hdmarx);
        for (uint16_t i = 0; i + 13 < recv_len; i++) {
            if (uartGroup.STP.Receive_data[i] == 0xAA && uartGroup.STP.Receive_data[i+1] == 0xAA &&
                uartGroup.STP.Receive_data[i+2] == 0xAA && uartGroup.STP.Receive_data[i+3] == 0xAA) {
                uint16_t datalen = (uartGroup.STP.Receive_data[i+9] << 8) | uartGroup.STP.Receive_data[i+8]; // 低位在前
                uint16_t pack_len = 4 + 1 + 1 + 2 + 2 + datalen + 1;
                if (recv_len - i >= pack_len) {
                    memcpy(uartGroup.STP.prcData, &uartGroup.STP.Receive_data[i], pack_len);
                    break;
                }
            }
        }
        HAL_UART_Receive_DMA(&huart2, uartGroup.STP.Receive_data, uartGroup.STP.buff);
    }
}
  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */
  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles TIM8 break interrupt and TIM12 global interrupt.
  */
void TIM8_BRK_TIM12_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_BRK_TIM12_IRQn 0 */

  /* USER CODE END TIM8_BRK_TIM12_IRQn 0 */
  HAL_TIM_IRQHandler(&htim8);
  HAL_TIM_IRQHandler(&htim12);
  /* USER CODE BEGIN TIM8_BRK_TIM12_IRQn 1 */

  /* USER CODE END TIM8_BRK_TIM12_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */
  switch (UART4MODE)
  {
  case 0:
    if (__HAL_UART_GET_FLAG(&huart4, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart4);
        HAL_UART_DMAStop(&huart4);
        uint16_t recv_len = uartGroup.steerimu.buff - __HAL_DMA_GET_COUNTER(huart4.hdmarx);
        register uint16_t i = 0;
        while (recv_len - i >= uartGroup.steerimu.len) {
            if (uartGroup.steerimu.Receive_data[i] == 0x55 && uartGroup.steerimu.Receive_data[i+1] == 0x53) {
                memcpy(uartGroup.steerimu.prcData, &uartGroup.steerimu.Receive_data[i], uartGroup.steerimu.len);
                i += uartGroup.steerimu.len;
            } else i++;
        }
        memset(uartGroup.steerimu.Receive_data, 0, uartGroup.steerimu.buff);
        HAL_UART_Receive_DMA(&huart4, uartGroup.steerimu.Receive_data, uartGroup.steerimu.buff);
    }
    break;
  case 1:
    if (__HAL_UART_GET_FLAG(&huart4, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart4);
        HAL_UART_DMAStop(&huart4);
        uint16_t recv_len = uartGroup.DL20.buff - __HAL_DMA_GET_COUNTER(huart4.hdmarx);
        register uint16_t i = 0;
        while (recv_len - i >= uartGroup.DL20.len) {
            if (uartGroup.DL20.Receive_data[i] == 0x07 && uartGroup.DL20.Receive_data[i+1] == 0x19) {
                memcpy(uartGroup.DL20.prcData, &uartGroup.DL20.Receive_data[i], uartGroup.DL20.len);
                i += uartGroup.DL20.len; 
            } else i++;
        }
        memset(uartGroup.DL20.Receive_data, 0, uartGroup.DL20.buff);
        HAL_UART_Receive_DMA(&huart4, uartGroup.DL20.Receive_data, uartGroup.DL20.buff);
    }
    break;
  default:
    break;
  }

  /* USER CODE END UART4_IRQn 0 */
  HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN UART4_IRQn 1 */

  /* USER CODE END UART4_IRQn 1 */
}

/**
  * @brief This function handles UART5 global interrupt.
  */
void UART5_IRQHandler(void)
{
  /* USER CODE BEGIN UART5_IRQn 0 */
  switch(UART5MODE)
  {
  case 0:
    if (__HAL_UART_GET_FLAG(&huart5, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart5);
        HAL_UART_DMAStop(&huart5);
        uint16_t recv_len = uartGroup.camera.buff - __HAL_DMA_GET_COUNTER(huart5.hdmarx);
        register uint16_t i = 0;
        while (recv_len - i >= uartGroup.camera.len) {
            if (uartGroup.camera.Receive_data[i] == 0x07 && uartGroup.camera.Receive_data[i+1] == 0x19) {
                memcpy(uartGroup.camera.prcData, &uartGroup.camera.Receive_data[i], uartGroup.camera.len);
                i += uartGroup.camera.len; 
            } else i++; 
        }
        memset(uartGroup.camera.Receive_data, 0, uartGroup.camera.buff);
        HAL_UART_Receive_DMA(&huart5, uartGroup.camera.Receive_data, uartGroup.camera.buff);
    }
    break;
  case 1:
    if (__HAL_UART_GET_FLAG(&huart5, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart5);
        HAL_UART_DMAStop(&huart5);
        uint16_t recv_len = uartGroup.FOC.buff - __HAL_DMA_GET_COUNTER(huart5.hdmarx);
        register uint16_t i = 0;
        while (recv_len - i >= uartGroup.FOC.len) {
            if (uartGroup.FOC.Receive_data[i] == 0x07 && uartGroup.FOC.Receive_data[i+1] == 0x19) {
                memcpy(uartGroup.FOC.prcData, &uartGroup.FOC.Receive_data[i], uartGroup.FOC.len);
                i += uartGroup.FOC.len;
            } else i++;
        }
        memset(uartGroup.FOC.Receive_data, 0, uartGroup.FOC.buff);
        HAL_UART_Receive_DMA(&huart5, uartGroup.FOC.Receive_data, uartGroup.FOC.buff);
    }
    break;
  }
  /* USER CODE END UART5_IRQn 0 */
  HAL_UART_IRQHandler(&huart5);
  /* USER CODE BEGIN UART5_IRQn 1 */
  /* USER CODE END UART5_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

  /* USER CODE END DMA2_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
