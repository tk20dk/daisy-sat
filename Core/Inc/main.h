/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32f0xx_ll_crc.h"
#include "stm32f0xx_ll_crs.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_spi.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_gpio.h"

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

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
#define DIO1_RADIO868_Pin LL_GPIO_PIN_13
#define DIO1_RADIO868_GPIO_Port GPIOC
#define DIO1_RADIO868_EXTI_IRQn EXTI4_15_IRQn
#define BUSY_RADIO868_Pin LL_GPIO_PIN_14
#define BUSY_RADIO868_GPIO_Port GPIOC
#define NRST_RADIO868_Pin LL_GPIO_PIN_15
#define NRST_RADIO868_GPIO_Port GPIOC
#define RXEN_RADIO868_Pin LL_GPIO_PIN_0
#define RXEN_RADIO868_GPIO_Port GPIOA
#define TXEN_RADIO868_Pin LL_GPIO_PIN_1
#define TXEN_RADIO868_GPIO_Port GPIOA
#define DIO2_RADIO868_Pin LL_GPIO_PIN_2
#define DIO2_RADIO868_GPIO_Port GPIOA
#define NSS_RADIO868_Pin LL_GPIO_PIN_3
#define NSS_RADIO868_GPIO_Port GPIOA
#define RADIO_NSS_Pin LL_GPIO_PIN_4
#define RADIO_NSS_GPIO_Port GPIOA
#define RADIO_DIO1_Pin LL_GPIO_PIN_0
#define RADIO_DIO1_GPIO_Port GPIOB
#define RADIO_DIO1_EXTI_IRQn EXTI0_1_IRQn
#define RADIO_BUSY_Pin LL_GPIO_PIN_1
#define RADIO_BUSY_GPIO_Port GPIOB
#define RADIO_NRST_Pin LL_GPIO_PIN_2
#define RADIO_NRST_GPIO_Port GPIOB
#define RADIO_TXEN_Pin LL_GPIO_PIN_10
#define RADIO_TXEN_GPIO_Port GPIOB
#define RADIO_DIO2_Pin LL_GPIO_PIN_11
#define RADIO_DIO2_GPIO_Port GPIOB
#define RADIO_RXEN_Pin LL_GPIO_PIN_12
#define RADIO_RXEN_GPIO_Port GPIOB
#define LED1_BLUE_Pin LL_GPIO_PIN_13
#define LED1_BLUE_GPIO_Port GPIOB
#define HMI_STATUS_Pin LL_GPIO_PIN_14
#define HMI_STATUS_GPIO_Port GPIOB
#define HMI_ERROR_Pin LL_GPIO_PIN_15
#define HMI_ERROR_GPIO_Port GPIOB
#define LED2_BLUE_Pin LL_GPIO_PIN_8
#define LED2_BLUE_GPIO_Port GPIOA
#define LED2_GREEN_Pin LL_GPIO_PIN_9
#define LED2_GREEN_GPIO_Port GPIOA
#define LED2_RED_Pin LL_GPIO_PIN_10
#define LED2_RED_GPIO_Port GPIOA
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
