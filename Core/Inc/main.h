/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "GeneralDefines.h"
#include "Ticks.h"
//#include "EEPROM_INTERNA.h"
#include "Def_System.h"
#include "Def_ProtocolSlave.h"
#include "LED.h"
//#include "ElectroV.h"
#include "Def_Standby.h"
#include "Def_Control.h"
#include "Def_RS485.h"
#include "AD5676.h"
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
void MX_USART1_UART_Init(void);
void MX_UART4_Init(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PS1_FAN_FAIL_Pin GPIO_PIN_2
#define PS1_FAN_FAIL_GPIO_Port GPIOE
#define LED_TEMP_2ERR_Pin GPIO_PIN_4
#define LED_TEMP_2ERR_GPIO_Port GPIOE
#define LED_AC2_ERR_Pin GPIO_PIN_5
#define LED_AC2_ERR_GPIO_Port GPIOE
#define LED_FAN_2ERR_Pin GPIO_PIN_6
#define LED_FAN_2ERR_GPIO_Port GPIOE
#define PS1_TEMP_ALARM_Pin GPIO_PIN_13
#define PS1_TEMP_ALARM_GPIO_Port GPIOC
#define ON_OFF_Pin GPIO_PIN_14
#define ON_OFF_GPIO_Port GPIOC
#define PS1_AC_FAIL_Pin GPIO_PIN_15
#define PS1_AC_FAIL_GPIO_Port GPIOC
#define LED_TEMP_1ERR_Pin GPIO_PIN_0
#define LED_TEMP_1ERR_GPIO_Port GPIOC
#define LED_AC1_ERR_Pin GPIO_PIN_1
#define LED_AC1_ERR_GPIO_Port GPIOC
#define LED_FAN1_ERR_Pin GPIO_PIN_2
#define LED_FAN1_ERR_GPIO_Port GPIOC
#define FAN_PS_EXIST_Pin GPIO_PIN_3
#define FAN_PS_EXIST_GPIO_Port GPIOC
#define PS2_AC_FAIL_Pin GPIO_PIN_0
#define PS2_AC_FAIL_GPIO_Port GPIOA
#define PS2_TEMP_ALARM_Pin GPIO_PIN_1
#define PS2_TEMP_ALARM_GPIO_Port GPIOA
#define PS2_FAN_FAIL_Pin GPIO_PIN_2
#define PS2_FAN_FAIL_GPIO_Port GPIOA
#define PS2_EN_Pin GPIO_PIN_3
#define PS2_EN_GPIO_Port GPIOA
#define CS_DAC_Pin GPIO_PIN_4
#define CS_DAC_GPIO_Port GPIOA
#define CLK_DAC_Pin GPIO_PIN_5
#define CLK_DAC_GPIO_Port GPIOA
#define MOSI_DAC_Pin GPIO_PIN_7
#define MOSI_DAC_GPIO_Port GPIOA
#define CS_DAC_FAN_Pin GPIO_PIN_4
#define CS_DAC_FAN_GPIO_Port GPIOC
#define CS_DAC_FANC5_Pin GPIO_PIN_5
#define CS_DAC_FANC5_GPIO_Port GPIOC
#define LOAD_DAC_Pin GPIO_PIN_0
#define LOAD_DAC_GPIO_Port GPIOB
#define EN_24V_FAN_Pin GPIO_PIN_1
#define EN_24V_FAN_GPIO_Port GPIOB
#define EN_24V_DIG_Pin GPIO_PIN_7
#define EN_24V_DIG_GPIO_Port GPIOE
#define EN_APL_5_Pin GPIO_PIN_8
#define EN_APL_5_GPIO_Port GPIOE
#define EN_APL_6_Pin GPIO_PIN_9
#define EN_APL_6_GPIO_Port GPIOE
#define EN_APL_7_Pin GPIO_PIN_10
#define EN_APL_7_GPIO_Port GPIOE
#define EN_APL_8_Pin GPIO_PIN_11
#define EN_APL_8_GPIO_Port GPIOE
#define EN_APL_1_Pin GPIO_PIN_12
#define EN_APL_1_GPIO_Port GPIOE
#define EN_APL_2_Pin GPIO_PIN_13
#define EN_APL_2_GPIO_Port GPIOE
#define EN_APL_3_Pin GPIO_PIN_14
#define EN_APL_3_GPIO_Port GPIOE
#define EN_APL_4_Pin GPIO_PIN_15
#define EN_APL_4_GPIO_Port GPIOE
#define EN_PS_GLOBAL_Pin GPIO_PIN_10
#define EN_PS_GLOBAL_GPIO_Port GPIOB
#define BTN_Pin GPIO_PIN_11
#define BTN_GPIO_Port GPIOB
#define MEM_CS_Pin GPIO_PIN_12
#define MEM_CS_GPIO_Port GPIOB
#define MEM_SCK_Pin GPIO_PIN_13
#define MEM_SCK_GPIO_Port GPIOB
#define MEM_MISO_Pin GPIO_PIN_14
#define MEM_MISO_GPIO_Port GPIOB
#define MEM_MOSI_Pin GPIO_PIN_15
#define MEM_MOSI_GPIO_Port GPIOB
#define MEM_WP_Pin GPIO_PIN_8
#define MEM_WP_GPIO_Port GPIOD
#define PS2_A1_Pin GPIO_PIN_9
#define PS2_A1_GPIO_Port GPIOD
#define PS2_A0_Pin GPIO_PIN_10
#define PS2_A0_GPIO_Port GPIOD
#define PG_PLT_8_Pin GPIO_PIN_11
#define PG_PLT_8_GPIO_Port GPIOD
#define PG_PLT_7_Pin GPIO_PIN_12
#define PG_PLT_7_GPIO_Port GPIOD
#define PG_PLT_6_Pin GPIO_PIN_13
#define PG_PLT_6_GPIO_Port GPIOD
#define PG_PLT_5_Pin GPIO_PIN_14
#define PG_PLT_5_GPIO_Port GPIOD
#define PG_24_FAN_Pin GPIO_PIN_15
#define PG_24_FAN_GPIO_Port GPIOD
#define PS1_A1_Pin GPIO_PIN_6
#define PS1_A1_GPIO_Port GPIOC
#define PS1_A0_Pin GPIO_PIN_7
#define PS1_A0_GPIO_Port GPIOC
#define PG_PLT_4_Pin GPIO_PIN_8
#define PG_PLT_4_GPIO_Port GPIOC
#define PG_PLT_3_Pin GPIO_PIN_9
#define PG_PLT_3_GPIO_Port GPIOC
#define PG_PLT_2_Pin GPIO_PIN_8
#define PG_PLT_2_GPIO_Port GPIOA
#define PG_PLT_1_Pin GPIO_PIN_10
#define PG_PLT_1_GPIO_Port GPIOA
#define EN_485_Pin GPIO_PIN_15
#define EN_485_GPIO_Port GPIOA
#define TX_485_Pin GPIO_PIN_10
#define TX_485_GPIO_Port GPIOC
#define RX_485_Pin GPIO_PIN_11
#define RX_485_GPIO_Port GPIOC
#define SEL_2_Pin GPIO_PIN_5
#define SEL_2_GPIO_Port GPIOD
#define SEL_1_Pin GPIO_PIN_6
#define SEL_1_GPIO_Port GPIOD
#define LED_G_Pin GPIO_PIN_7
#define LED_G_GPIO_Port GPIOD
#define LED_Y_Pin GPIO_PIN_4
#define LED_Y_GPIO_Port GPIOB
#define LED_ALARM_Pin GPIO_PIN_5
#define LED_ALARM_GPIO_Port GPIOB
#define ALARMA__IN_Pin GPIO_PIN_8
#define ALARMA__IN_GPIO_Port GPIOB
#define ALARMA_OUT_Pin GPIO_PIN_9
#define ALARMA_OUT_GPIO_Port GPIOB
#define R_LED_Pin GPIO_PIN_0
#define R_LED_GPIO_Port GPIOE
#define G_LED_Pin GPIO_PIN_1
#define G_LED_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
