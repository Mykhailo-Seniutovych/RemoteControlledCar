/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RED_PWM_Pin GPIO_PIN_0
#define RED_PWM_GPIO_Port GPIOA
#define GREEN_PWM_Pin GPIO_PIN_1
#define GREEN_PWM_GPIO_Port GPIOA
#define BLUE_PWM_Pin GPIO_PIN_2
#define BLUE_PWM_GPIO_Port GPIOA
#define CSN_Pin GPIO_PIN_0
#define CSN_GPIO_Port GPIOB
#define CE_Pin GPIO_PIN_1
#define CE_GPIO_Port GPIOB
#define DRV_RIGHT_PNP1_Pin GPIO_PIN_12
#define DRV_RIGHT_PNP1_GPIO_Port GPIOB
#define DRV_RIGHT_PNP2_Pin GPIO_PIN_13
#define DRV_RIGHT_PNP2_GPIO_Port GPIOB
#define DRV_LEFT_PNP1_Pin GPIO_PIN_14
#define DRV_LEFT_PNP1_GPIO_Port GPIOB
#define DRV_LEFT_PNP2_Pin GPIO_PIN_15
#define DRV_LEFT_PNP2_GPIO_Port GPIOB
#define DRV_RIGHT_NPN2_Pin GPIO_PIN_9
#define DRV_RIGHT_NPN2_GPIO_Port GPIOA
#define DRV_LEFT_NPN1_Pin GPIO_PIN_10
#define DRV_LEFT_NPN1_GPIO_Port GPIOA
#define DRV_LEFT_NPN2_Pin GPIO_PIN_11
#define DRV_LEFT_NPN2_GPIO_Port GPIOA
#define DRV_RIGHT_NPN1_Pin GPIO_PIN_5
#define DRV_RIGHT_NPN1_GPIO_Port GPIOB
#define MOUNT_HOR_Pin GPIO_PIN_6
#define MOUNT_HOR_GPIO_Port GPIOB
#define MOUNT_VERT_Pin GPIO_PIN_7
#define MOUNT_VERT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
