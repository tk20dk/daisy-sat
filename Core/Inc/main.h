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
#include "stm32f0xx_hal.h"

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
#define DIO1_RADIO868_Pin GPIO_PIN_13
#define DIO1_RADIO868_GPIO_Port GPIOC
#define DIO1_RADIO868_EXTI_IRQn EXTI4_15_IRQn
#define BUSY_RADIO868_Pin GPIO_PIN_14
#define BUSY_RADIO868_GPIO_Port GPIOC
#define NRST_RADIO868_Pin GPIO_PIN_15
#define NRST_RADIO868_GPIO_Port GPIOC
#define RXEN_RADIO868_Pin GPIO_PIN_0
#define RXEN_RADIO868_GPIO_Port GPIOA
#define TXEN_RADIO868_Pin GPIO_PIN_1
#define TXEN_RADIO868_GPIO_Port GPIOA
#define DIO2_RADIO868_Pin GPIO_PIN_2
#define DIO2_RADIO868_GPIO_Port GPIOA
#define NSS_RADIO868_Pin GPIO_PIN_3
#define NSS_RADIO868_GPIO_Port GPIOA
#define RADIO_NSS_Pin GPIO_PIN_4
#define RADIO_NSS_GPIO_Port GPIOA
#define RADIO_DIO1_Pin GPIO_PIN_0
#define RADIO_DIO1_GPIO_Port GPIOB
#define RADIO_DIO1_EXTI_IRQn EXTI0_1_IRQn
#define RADIO_BUSY_Pin GPIO_PIN_1
#define RADIO_BUSY_GPIO_Port GPIOB
#define RADIO_NRST_Pin GPIO_PIN_2
#define RADIO_NRST_GPIO_Port GPIOB
#define RADIO_TXEN_Pin GPIO_PIN_10
#define RADIO_TXEN_GPIO_Port GPIOB
#define RADIO_DIO2_Pin GPIO_PIN_11
#define RADIO_DIO2_GPIO_Port GPIOB
#define RADIO_RXEN_Pin GPIO_PIN_12
#define RADIO_RXEN_GPIO_Port GPIOB
#define LED1_BLUE_Pin GPIO_PIN_13
#define LED1_BLUE_GPIO_Port GPIOB
#define HMI_STATUS_Pin GPIO_PIN_14
#define HMI_STATUS_GPIO_Port GPIOB
#define HMI_ERROR_Pin GPIO_PIN_15
#define HMI_ERROR_GPIO_Port GPIOB
#define LED2_BLUE_Pin GPIO_PIN_8
#define LED2_BLUE_GPIO_Port GPIOA
#define LED2_GREEN_Pin GPIO_PIN_9
#define LED2_GREEN_GPIO_Port GPIOA
#define LED2_RED_Pin GPIO_PIN_10
#define LED2_RED_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
