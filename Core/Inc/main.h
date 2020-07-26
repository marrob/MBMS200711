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
 #define DEVICE_DEBUG_LEVEL    3


#if (DEVICE_DEBUG_LEVEL > 0)
#define  DeviceUsrLog(...)  {printf(__VA_ARGS__);\
                             printf("\r\n");}
#else
#define DeviceUsrLog(...)
#endif

#if (DEVICE_DEBUG_LEVEL > 1)

#define  DeviceErrLog(...)  {printf(VT100_ATTR_RED);\
                             printf("ERROR.DEVICE:") ;\
                             printf(__VA_ARGS__);\
                             printf(VT100_ATTR_RESET);\
                             printf("\r\n");}
#else
#define DeviceErrLog(...)
#endif

#if (DEVICE_DEBUG_LEVEL > 2)
#define  DeviceDbgLog(...)  {printf(VT100_ATTR_YELLOW);\
                             printf("DEBUG.DEVICE:") ;\
                             printf(__VA_ARGS__);\
                             printf(VT100_ATTR_RESET);\
                             printf("\r\n");}
#else
#define DeviceDbgLog(...)
#endif
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LIVE_LED_Pin GPIO_PIN_13
#define LIVE_LED_GPIO_Port GPIOC
#define MAX_CS_Pin GPIO_PIN_12
#define MAX_CS_GPIO_Port GPIOB
#define MAX_INT_Pin GPIO_PIN_8
#define MAX_INT_GPIO_Port GPIOA
#define MAX_SHDN_Pin GPIO_PIN_9
#define MAX_SHDN_GPIO_Port GPIOA
#define USB_PULL_Pin GPIO_PIN_15
#define USB_PULL_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

#define DEVICE_FW           0x200720
#define DEVICE_NAME         "MBMS200711"
#define DEVICE_PCB          "V00"
#define DEVICE_MNF          "KONVOLUCIO BT"
#define DEVICE_NAME_SIZE    32
#define DEVICE_FW_SIZE      sizeof(DEVICE_FW)
#define DEVICE_PCB_SIZE     sizeof(DEVICE_PCB)
#define DEVICE_MNF_SIZE     sizeof(DEVICE_MNF)

#define DEVICE_OK           0
#define DEVICE_FAIL         1


/* VT100 ---------------------------------------------------------------------*/
/*
 * https://www.csie.ntu.edu.tw/~r92094/c++/VT100.html
 * http://www.termsys.demon.co.uk/vtansi.htm
 */
#define VT100_CLEARSCREEN   "\033[2J"
#define VT100_CURSORHOME    "\033[H"
#define VT100_ATTR_RESET    "\033[0m"
#define VT100_ATTR_RED      "\033[31m"
#define VT100_ATTR_GREEN    "\033[32m"
#define VT100_ATTR_YELLOW   "\033[33m"
#define VT100_CUP(__v__,__h__)    ("\033["__v__";"__h__"H") /*Cursor Position*/
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
