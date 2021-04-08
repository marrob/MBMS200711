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
#include "vt100.h"
#include <stm32f1xx_hal.h>
#include "StringPlus.h"
/* Private define ------------------------------------------------------------*/
#define ASCI_MODEL    0x15
#define ASCI_VERSION  0x17
#define ASCI_TX_BUFFER_SIZE 28
#define ASCI_RX_BUFFER_SIZE 62

#define ASCI_OK                 0x00
#define ASCI_IO_ERROR           0x01
#define ASCI_TIMEOUT_ERROR      0x02
#define ASCI_LOAD_VERIFY_ERROR  0x03

#define ASCI_RX_TIMEOUT_CYCLE 1000
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
 #define ASCI_DEBUG_LEVEL    3


#if (ASCI_DEBUG_LEVEL > 0)
#define AsciUsrLog(...)     {printf(__VA_ARGS__);\
                             printf("\r\n");}
#else
#define DeviceUsrLog(...)
#endif

#if (ASCI_DEBUG_LEVEL > 1)

#define  AsciErrLog(...)    {printf(VT100_ATTR_RED);\
                             printf("ERROR.ASCI:") ;\
                             printf(__VA_ARGS__);\
                             printf(VT100_ATTR_RESET);\
                             printf("\r\n");}
#else
#define DeviceErrLog(...)
#endif

#if (ASCI_DEBUG_LEVEL > 2)
#define  AsciDbgLog(...)    {printf(VT100_ATTR_YELLOW);\
                             printf("DEBUG.ASCI:") ;\
                             printf(__VA_ARGS__);\
                             printf(VT100_ATTR_RESET);\
                             printf("\r\n");}
#else
#define AsciDbgLog(...)
#endif
/* USER CODE END EM */

/* Exported functions ------------------------------------------------------- */
uint8_t AsciIoWrite(uint8_t *writeBuffer, uint8_t size);
uint8_t AsciIoReadRegU8(uint8_t regAddr,  uint8_t *byte);
uint8_t AsciIoReadReg(uint8_t regAddr,  uint8_t *rxBuffer, uint8_t size);
uint8_t AsciIoGetModel(void);

void AsciIoPEC_CalcTest(void);


uint8_t AsciIoUartWriteRead(uint8_t *tx, uint8_t tx_size, uint8_t *rx, uint8_t rx_size);
uint8_t AsciIoSlaveReadReg(uint8_t slave, uint8_t regAddr, uint8_t slaveCnt, uint8_t *rx, uint8_t size);

uint8_t AsciAdapterInit(void);
void AsciAdapterTask(void);

uint8_t AsciIoWriteSlaveReg(uint8_t slave, uint8_t regAddr, uint16_t value );

#endif /* SRC_ASCI_H_ */

/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/
