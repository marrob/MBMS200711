/*
 * asci.h
 *
 *  Created on: Apr 3, 2021
 *      Author: Margit Robert
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SRC_ASCI_H_
#define SRC_ASCI_H_
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stm32f1xx_hal.h>
/* Private define ------------------------------------------------------------*/
#define MAX_ASCI_MODEL    0x15
#define MAX_ASCI_VERSION  0x17

#define MAX_ASCI_OK       0x00
#define MAX_ASCI_IO_ERROR 0x01
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
 #define DEVICE_DEBUG_LEVEL    3


#if (ASCI_DEBUG_LEVEL > 0)
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

/* Exported functions ------------------------------------------------------- */
uint8_t AsciIoWrite(uint8_t *writeBuffer, uint8_t size);
uint8_t AsciIoReadRegU8(uint8_t regAddr,  uint8_t *byte);
uint8_t AsciIoReadReg(uint8_t regAddr,  uint8_t *rxBuffer, uint8_t size);
uint8_t AsciIoGetModel(void);

uint8_t AsciAdapterInit(void);


#endif /* SRC_ASCI_H_ */

/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/
