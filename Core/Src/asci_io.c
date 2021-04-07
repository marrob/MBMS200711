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
char strBuff[80];
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

uint8_t AsciIoWrite(uint8_t *writeBuffer, uint8_t size)
{
  uint8_t status = ASCI_OK;

  HAL_GPIO_WritePin(MAX_CS_GPIO_Port, MAX_CS_Pin, GPIO_PIN_RESET);

  if(HAL_SPI_Transmit(&hspi2, writeBuffer, size, 100)!= HAL_OK)
  {
    status = ASCI_IO_ERROR;
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
  uint8_t status = ASCI_OK;
  uint8_t txBuffer[size + 1];
  uint8_t rxTemp[size + 1];

  HAL_GPIO_WritePin(MAX_CS_GPIO_Port, MAX_CS_Pin, GPIO_PIN_RESET);

  memset(txBuffer,0xAA, size + 1);
  txBuffer[0] = regAddr;
  if(HAL_SPI_TransmitReceive(&hspi2, txBuffer, rxTemp, size + 1 , 100)!= HAL_OK)
  {
    status = ASCI_IO_ERROR;
  }

  memcpy(rxBuffer, rxTemp + 1, size);

  HAL_GPIO_WritePin(MAX_CS_GPIO_Port, MAX_CS_Pin, GPIO_PIN_SET);

  return status;
}




uint8_t AsciIoWriteRead(uint8_t *tx, uint8_t tx_size, uint8_t *rx, uint8_t rx_size){

  uint8_t txBuff[28];
  uint8_t rxBuff[62];

  memset(rxBuff,0x00, sizeof(rxBuff));

  //Enable Transmit Preambles mode
  memcpy(txBuff, (uint8_t[]){0x30}, 1);
  AsciIoWrite(txBuff, 1);

  //Clear receive buffer
  memcpy(txBuff, (uint8_t[]){0xE0}, 1);
  AsciIoWrite(txBuff, 1);

  //Load sequence into Tx queue
  AsciIoWrite(tx, tx_size);
  StringPlusDataToHexaString(tx,strBuff, tx_size);
  AsciUsrLog("Load into the Tx queue:%s",strBuff);

  //Verify content of the load queue
  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0xC1, rxBuff, tx_size-1);
  StringPlusDataToHexaString(rxBuff,strBuff, tx_size-1);
  AsciUsrLog("Verify content of the load queue:%s",strBuff);

  //Transmitt
  memcpy(txBuff, (uint8_t[]){0xB0}, 1);
  AsciIoWrite(txBuff, 1);

  //Poll Rx_Stop_Status bit
  int16_t i=0;
  do
  {
    AsciIoReadReg(0x01, rxBuff, 1);
    sprintf(strBuff,"%03d Read RX_Status:0x%02X",i,rxBuff[0]);
    AsciDbgLog(strBuff);
    i++;
  }while(rxBuff[0]!=0x21 && i < ASCI_RX_TIMEOUT_CYCLE);

  if(i >= ASCI_RX_TIMEOUT_CYCLE){
    AsciErrLog("Timeout.");
    //return ASCI_TIMEOUT_ERROR;
  }

  //Receive
  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0x93, rxBuff,tx_size-1);
  StringPlusDataToHexaString(rxBuff,strBuff, tx_size-1);
  AsciUsrLog("Read from Rx queue:%s",strBuff);


  //Check for receive buffer errors
  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0x09, rxBuff,1);
  if(rxBuff[0]!=0 || rxBuff[0]!=0x02 )
    AsciErrLog("Receive Buffer Error, 0x%02X",rxBuff[0]);


  return ASCI_OK;
}



uint8_t AsciIoGetModel(void)
{

  uint8_t modelNumber[] = {0}; /*Defult is 0x84*/
  AsciIoReadReg(ASCI_MODEL, modelNumber, sizeof(modelNumber));
  DeviceDbgLog("Model Number 0x%02X", modelNumber[0]);
  uint8_t versionNumber[] = {0}; /*Defult is 0x12*/
  AsciIoReadReg(ASCI_VERSION, versionNumber, sizeof(versionNumber));
  DeviceDbgLog("Version Number 0x%02X", versionNumber[0]);
  return 0;
}






/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/
