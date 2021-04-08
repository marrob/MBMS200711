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
uint32_t transaction;
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




uint8_t AsciIoUartWriteRead(uint8_t *tx, uint8_t tx_size, uint8_t *rx, uint8_t rx_size){

  uint8_t txBuff[28];
  uint8_t rxBuff[62];

  transaction++;

  memset(rxBuff,0x00, sizeof(rxBuff));

  //A0x30-nak nincs dokumentálva a funkciója
  //Ez sztm 0x20-nak kellene lennie
  //Clear transmit buffer
  memcpy(txBuff, (uint8_t[]){0x20}, 1);
  AsciIoWrite(txBuff, 1);

  //Clear receive buffer
  memcpy(txBuff, (uint8_t[]){0xE0}, 1);
  AsciIoWrite(txBuff, 1);

  //Load sequence into Tx queue
  AsciIoWrite(tx, tx_size);
  StringPlusDataToHexaString(tx,strBuff, tx_size);
  AsciUsrLog("TR:%04ld-Load:%s",transaction,strBuff);

  //Verify content of the load queue
  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0xC1, rxBuff, tx_size-1);
  StringPlusDataToHexaString(rxBuff,strBuff, tx_size-1);
  AsciUsrLog("TR:%04ld-Verify:%s",transaction, strBuff);

  //Transmitt
  memcpy(txBuff, (uint8_t[]){0xB0}, 1);
  AsciIoWrite(txBuff, 1);


  //Poll Rx_Stop_Status bit
  //Ha jött be üzenet az Rx Queue-ba, akkor Rx_Stop-ot jelez
  int16_t i=0;
  do
  {
    AsciIoReadReg(0x01, rxBuff, 1);
    //sprintf(strBuff,"%03d Read RX_Status:0x%02X",i,rxBuff[0]);
    //AsciDbgLog(strBuff);
    i++;
  }while(rxBuff[0]!=0x12 && i < ASCI_RX_TIMEOUT_CYCLE);

  if(i >= ASCI_RX_TIMEOUT_CYCLE){
    AsciErrLog("Timeout.");
    //return ASCI_TIMEOUT_ERROR;
  }


  if(rx != NULL)
  {
    //Receive
    memset(rx,0xCC, rx_size);
    AsciIoReadReg(0x93, rx, rx_size);
    StringPlusDataToHexaString(rx,strBuff, rx_size);
    AsciUsrLog("TR:%04ld-Read:%s",transaction,strBuff);
  }

  //Check for receive buffer errors
  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0x09, rxBuff,1);
  if(rxBuff[0]!=0  && rxBuff[0]!=0x02 )
    AsciErrLog("TR:%04ld-Receive Buffer Error, 0x%02X",transaction,rxBuff[0]);

  AsciUsrLog("TR:%04ld--- Completed ---",transaction);

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


void AsciIoPEC_CalcTest(void){
  DeviceDbgLog( "PEC for 0x03,0x12,0x00: 0x%02X (expected: 0xCB)", AsciIoPEC((uint8_t[]){0x03,0x12,0x00}, 3, 0x00));
  DeviceDbgLog( "PEC for 0x02,0x12,0xB1,0xB2: 0x%02X (expected: 0xC4)", AsciIoPEC((uint8_t[]){0x02,0x12,0xB1,0xB2}, 4, 0x00));

}

uint8_t AsciIoPEC(uint8_t *data, uint8_t size, uint8_t crc)
{
  // CRCByte is initialized to 0 for each ByteList in this implementation, where
  // ByteList contains all bytes of a single command. It is passed into the
  // function in case a partial ByteList calculation is needed.
  // Data is transmitted and calculated in LSb first format
  // Polynomial = x^8+x^6+x^3+x^2+1
  //*POLY = &HB2 // 10110010b for LSb first
    uint8_t poly = 0xB2;
  //Loop once for each byte in the ByteList
  //*For ByteCounter = 0 to (NumberOfBytes – 1)

  for(uint8_t  byte =0; byte < size; byte++)
  {
    //CRCByte = CRCByte XOR ByteList(Counter1)
    crc = crc ^ data[byte];
    //Process each of the 8 CRCByte remainder bits

    for(uint8_t bit = 0; bit <=7; bit++)
    {
      // The LSb should be shifted toward the highest order polynomial
      // coefficient. This is a right shift for data stored LSb to the right
      // and POLY having high order coefficients stored to the right.
      // Determine if LSb = 1 prior to right shift
      //If (CRCByte AND &H01) = 1 Then
      if(crc & 0x01){
       // When LSb = 1, right shift and XOR CRCByte value with 8 LSbs
       // of the polynomial coefficient constant. “/ 2” must be a true right
       // shift in the target CPU to avoid rounding problems.
       //CRCByte = ((CRCByte / 2) XOR POLY)
        crc = ((crc/2)^poly);
      }
      else
      {
        //When LSb = 0, right shift by 1 bit. “/ 2” must be a true right
        // shift in the target CPU to avoid rounding problems.
        //CRCByte = (CRCByte / 2)
        crc = crc/2;
      }
      //Truncate the CRC value to 8 bits if necessary
      //CRCByte = CRCByte AND &HFF
      crc = crc & 0xFF;
      //Proceed to the next bit
      //Next BitCounter
    }
    //Operate on the next data byte in the ByteList
    //Next ByteCounter
  }
  // All calculations done; CRCByte value is the CRC byte for ByteList() and
  // the initial CRCByte value
  //Return CRCByte

  return crc;
}





/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/
