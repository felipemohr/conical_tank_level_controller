/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define SETPOINT_POT_Pin GPIO_PIN_0
#define SETPOINT_POT_GPIO_Port GPIOA
#define SETPOINT_BUT_Pin GPIO_PIN_3
#define SETPOINT_BUT_GPIO_Port GPIOA
#define FLOW_SENSOR_Pin GPIO_PIN_1
#define FLOW_SENSOR_GPIO_Port GPIOB
#define FLOW_SENSOR_EXTI_IRQn EXTI1_IRQn
#define TRIG_Pin GPIO_PIN_10
#define TRIG_GPIO_Port GPIOB
#define ECHO_Pin GPIO_PIN_11
#define ECHO_GPIO_Port GPIOB
#define VALVE_DIR_Pin GPIO_PIN_12
#define VALVE_DIR_GPIO_Port GPIOB
#define VALVE_PWM_Pin GPIO_PIN_13
#define VALVE_PWM_GPIO_Port GPIOB
#define PUMP_PWM_Pin GPIO_PIN_15
#define PUMP_PWM_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_8
#define LCD_RS_GPIO_Port GPIOA
#define LCD_EN_Pin GPIO_PIN_9
#define LCD_EN_GPIO_Port GPIOA
#define LCD_D4_Pin GPIO_PIN_10
#define LCD_D4_GPIO_Port GPIOA
#define LCD_D5_Pin GPIO_PIN_11
#define LCD_D5_GPIO_Port GPIOA
#define LCD_D6_Pin GPIO_PIN_12
#define LCD_D6_GPIO_Port GPIOA
#define LCD_D7_Pin GPIO_PIN_15
#define LCD_D7_GPIO_Port GPIOA
#define PUMP_EN_Pin GPIO_PIN_5
#define PUMP_EN_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define SAMPLING_FREQUENCY 10.0f
#define LCD_UPDATE_TIME 1.0f
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
