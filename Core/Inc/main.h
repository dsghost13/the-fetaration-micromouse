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
#define V_BATT_Pin GPIO_PIN_1
#define V_BATT_GPIO_Port GPIOA
#define MR_PWM_Pin GPIO_PIN_2
#define MR_PWM_GPIO_Port GPIOA
#define ML_PWM_Pin GPIO_PIN_3
#define ML_PWM_GPIO_Port GPIOA
#define RECIV_FR_Pin GPIO_PIN_4
#define RECIV_FR_GPIO_Port GPIOA
#define RECIV_R_Pin GPIO_PIN_5
#define RECIV_R_GPIO_Port GPIOA
#define ML_ENC_A_Pin GPIO_PIN_6
#define ML_ENC_A_GPIO_Port GPIOA
#define ML_ENC_B_Pin GPIO_PIN_7
#define ML_ENC_B_GPIO_Port GPIOA
#define RECIV_L_Pin GPIO_PIN_0
#define RECIV_L_GPIO_Port GPIOB
#define RECIV_FL_Pin GPIO_PIN_1
#define RECIV_FL_GPIO_Port GPIOB
#define EMIT_R_Pin GPIO_PIN_10
#define EMIT_R_GPIO_Port GPIOB
#define EMIT_L_Pin GPIO_PIN_11
#define EMIT_L_GPIO_Port GPIOB
#define EMIT_FL_Pin GPIO_PIN_12
#define EMIT_FL_GPIO_Port GPIOB
#define MR_FWD_Pin GPIO_PIN_13
#define MR_FWD_GPIO_Port GPIOB
#define ML_BWD_Pin GPIO_PIN_14
#define ML_BWD_GPIO_Port GPIOB
#define MR_BWD_Pin GPIO_PIN_15
#define MR_BWD_GPIO_Port GPIOB
#define ML_FWD_Pin GPIO_PIN_8
#define ML_FWD_GPIO_Port GPIOA
#define EMIT_FR_Pin GPIO_PIN_5
#define EMIT_FR_GPIO_Port GPIOB
#define MR_ENC_B_Pin GPIO_PIN_6
#define MR_ENC_B_GPIO_Port GPIOB
#define MR_ENC_A_Pin GPIO_PIN_7
#define MR_ENC_A_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_8
#define BUZZER_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern const float LOOP_FREQUENCY;
extern const float LOOP_INTERVAL;

extern const float WHEEL_DIAMETER;
extern const float ENCODER_PULSES;
extern const float GEAR_RATIO;

extern const float MOUSE_RADIUS;
extern const float TRACK_WIDTH;

extern const float PI;
extern const float MM_PER_COUNT;
extern const float MM_PER_COUNT_RIGHT;
extern const float MM_PER_COUNT_LEFT;
extern const float DEG_PER_MM_DIF;

extern const float FULL_CELL_MM;
extern const float HALF_CELL;
// extern const float DIAG_CELL;
extern const float BACK_TO_MOTOR;
extern const float BACK_WALL_TO_CENTER;
extern const float SENSING_POSITION;

extern const float MAX_ADC;
extern const float VREFINT;

extern const int SEARCH_FWD_SPEED;
extern const int SEARCH_FWD_ACCELERATION;
extern const int SEARCH_TURN_SPEED;
extern const int SEARCH_TURN_ACCELERATION;
extern const int SMOOTH_FWD_SPEED;
extern const int SMOOTH_FWD_ACCELERATION;
extern const int FAST_FWD_SPEED;
extern const int FAST_FWD_ACCELERATION;
extern const int FAST_TURN_SPEED;
extern const int FAST_TURN_ACCELERATION;

extern const int NOM_FORWARD;
extern const int NOM_DIAGONAL;

extern const int MOTOR_MAX_PWM;

extern const float SIDE_WALL_THRESHOLD_MM;
extern const float FRONT_WALL_THRESHOLD_MM;
extern const float DESIRED_WALL_DISTANCE_MM;
extern const float STEERING_KP;

extern const float FWD_KP;
extern const float FWD_KD;
extern const float ROT_KP;
extern const float ROT_KD;
extern const float SPEED_FF;
extern const float BIAS_FF;
extern const float ACC_FF;


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
