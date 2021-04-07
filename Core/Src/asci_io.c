/*
 * asci_low_level.c
 *
 *  Created on: Apr 3, 2021
 *      Author: Margit Robert
 */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include "asci.h"
#include "main.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

uint8_t AsciIoWrite(uint8_t *writeBuffer, uint8_t size)
{
  uint8_t status = MAX_ASCI_OK;

  HAL_GPIO_WritePin(MAX_CS_GPIO_Port, MAX_CS_Pin, GPIO_PIN_RESET);

  if(HAL_SPI_Transmit(&hspi2, writeBuffer, size, 100)!= HAL_OK)
  {
    status = MAX_ASCI_IO_ERROR;
  }

  HAL_GPIO_WritePin(MAX_CS_GPIO_Port, MAX_CS_Pin, GPIO_PIN_SET);

  return status;
}


uint8_t AsciIoReadRegU8(uint8_t regAddr,  uint8_t *byte)
{
  return AsciIoReadReg(regAddr,  byte, 1 );
}

uint8_t AsciIoReadReg(uint8_t regAddr,  uint8_t *rxBuffer, uint8_t size)
{
  uint8_t status = MAX_ASCI_OK;
  uint8_t txBuffer[size + 1];
  uint8_t rxTemp[size + 1];

  HAL_GPIO_WritePin(MAX_CS_GPIO_Port, MAX_CS_Pin, GPIO_PIN_RESET);

  memset(txBuffer,0xAA, size + 1);
  txBuffer[0] = regAddr;
  if(HAL_SPI_TransmitReceive(&hspi2, txBuffer, rxTemp, size + 1 , 100)!= HAL_OK)
  {
    status = MAX_ASCI_IO_ERROR;
  }

  memcpy(rxBuffer, rxTemp + 1, size);

  HAL_GPIO_WritePin(MAX_CS_GPIO_Port, MAX_CS_Pin, GPIO_PIN_SET);

  return status;
}

uint8_t AsciIoGetModel(void)
{

  uint8_t modelNumber[] = {0}; /*Defult is 0x84*/
  AsciIoReadReg(MAX_ASCI_MODEL, modelNumber, sizeof(modelNumber));
  DeviceDbgLog("Model Number 0x%02X", modelNumber[0]);
  uint8_t versionNumber[] = {0}; /*Defult is 0x12*/
  AsciIoReadReg(MAX_ASCI_VERSION, versionNumber, sizeof(versionNumber));
  DeviceDbgLog("Version Number 0x%02X", versionNumber[0]);
  return 0;
}






/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/
