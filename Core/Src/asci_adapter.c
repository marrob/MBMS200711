/*
 * asci_adapter.c
 *
 *  Created on: Apr 3, 2021
 *      Author: Margit Robert
 */

/* Includes ------------------------------------------------------------------*/
#include "asci.h"
#include <string.h>
#include <stdio.h>
#include "common.h"
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
  char strBuff[80];
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
uint8_t AsciAdapterInit(void)
{
  uint8_t rxBuff[16];
  uint8_t txBuff[16];

  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0x09,rxBuff,1);
  sprintf(strBuff,"Read Tx Interrupt Register:0x%02X",rxBuff[0]);
  AsciDbgLog(strBuff);

  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0x15,rxBuff,1);
  sprintf(strBuff,"Read Model Register:0x%02X",rxBuff[0]);
  AsciDbgLog(strBuff);

  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0x17,rxBuff,1);
  sprintf(strBuff,"Read Version Register:0x%02X",rxBuff[0]);
  AsciDbgLog(strBuff);

  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0x13,rxBuff,1);
  sprintf(strBuff,"Read FEMA Register:0x%02X",rxBuff[0]);
  AsciDbgLog(strBuff);

  memcpy(txBuff, (uint8_t[]){0x08,0x00}, 2);
  AsciIoWrite(txBuff, 2);

  memcpy(txBuff, (uint8_t[]){0x08,0x00}, 2);
  AsciIoWrite(txBuff, 2);

  memcpy(txBuff, (uint8_t[]){0x04,0x02}, 2);
  AsciIoWrite(txBuff, 2);

  memcpy(txBuff, (uint8_t[]){0x06,0x00}, 2);
  AsciIoWrite(txBuff, 2);

  memcpy(txBuff, (uint8_t[]){0xC0,0x60}, 2);
  AsciIoWrite(txBuff, 2);

  memcpy(txBuff, (uint8_t[]){0x0E,0x10}, 2);
  AsciIoWrite(txBuff, 2);

  memcpy(txBuff, (uint8_t[]){0x10,0x05}, 2);
  AsciIoWrite(txBuff, 2);

  memcpy(txBuff, (uint8_t[]){0x30}, 1);
  AsciIoWrite(txBuff, 1);

  memcpy(txBuff, (uint8_t[]){0xE0}, 1);
  AsciIoWrite(txBuff, 1);

  memcpy(txBuff, (uint8_t[]){0x93}, 1);
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

  if(i >= ASCI_RX_TIMEOUT_CYCLE)
    AsciErrLog("Timeout.");


 //HELLOALL
 AsciIoWriteRead((uint8_t[]){0xC0,0x03,0x57,0x00,0x00}, 5, NULL,0);

 AsciIoWriteRead((uint8_t[]){0xC0,0x07,0x04,0x1B,0x00,0x80,0xA2,0x00,0x00},9, NULL,0);

  /*
  //Enable Transmit Preambles mode
  memcpy(txBuff, (uint8_t[]){0x30}, 1);
  AsciIoWrite(txBuff, 1);

  //Clear receive buffer
  memcpy(txBuff, (uint8_t[]){0xE0}, 1);
  AsciIoWrite(txBuff, 1);

  //Load HELLOALL command sequence into tx queue
  memcpy(txBuff, (uint8_t[]){0xC0,0x03,0x57,0x00,0x00}, 5);
  AsciIoWrite(txBuff, 5);
  StringPlusDataToHexaString(txBuff,strBuff, 5);
  AsciUsrLog("Load HELLOALL:%s",strBuff);

  //Verify content of the load queue
  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0xC1, rxBuff,4);
  StringPlusDataToHexaString(rxBuff,strBuff, 4);
  AsciUsrLog("Verify content of the load queue:%s",strBuff);

  //Transmitt
  memcpy(txBuff, (uint8_t[]){0xB0}, 1);
  AsciIoWrite(txBuff, 1);

  //Poll Rx_Stop_Status bit
  i=0;
  do
  {
    AsciIoReadReg(0x01, rxBuff, 1);
    sprintf(strBuff,"%03d Read RX_Status:0x%02X",i,rxBuff[0]);
    AsciDbgLog(strBuff);
    i++;
  }while(rxBuff[0]!=0x21 && i < ASCI_RX_TIMEOUT_CYCLE);

  if(i >= ASCI_RX_TIMEOUT_CYCLE)
    AsciErrLog("Timeout.");

  //Receive
  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0x93, rxBuff,3);
  StringPlusDataToHexaString(rxBuff,strBuff, 3);
  AsciUsrLog("Read from Rx Queue:%s",strBuff);


  //Enable Transmit Preambles mode
  memcpy(txBuff, (uint8_t[]){0x30}, 1);
  AsciIoWrite(txBuff, 1);

  //Clear receive buffer
  memcpy(txBuff, (uint8_t[]){0xE0}, 1);
  AsciIoWrite(txBuff, 1);

  //Load xxxx command sequence into tx queue
  memcpy(txBuff, (uint8_t[]){0xC0,0x07,0x04,0x1B,0x00,0x80,0xA2,0x00,0x00}, 9);
  AsciIoWrite(txBuff, 9);
  StringPlusDataToHexaString(txBuff,strBuff, 9);
  AsciUsrLog("Load xxxx command sequence into tx queue:%s",strBuff);

  //Verify content of the load queue
  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0xC1, rxBuff,8);
  StringPlusDataToHexaString(rxBuff,strBuff, 8);
  AsciUsrLog("Verify content of the load queue:%s",strBuff);

  //Start transmitting
  memcpy(txBuff, (uint8_t[]){0xB0}, 1);
  AsciIoWrite(txBuff, 1);

  //Poll Rx_Stop_Status bit
  i=0;
  do
  {
    AsciIoReadReg(0x01, rxBuff, 1);
    sprintf(strBuff,"%03d Read RX_Status:0x%02X",i,rxBuff[0]);
    AsciDbgLog(strBuff);
    i++;
  }while(rxBuff[0]!=0x21 && i < ASCI_RX_TIMEOUT_CYCLE);

  if(i >= ASCI_RX_TIMEOUT_CYCLE)
    AsciErrLog("Timeout.");

  //Receive
  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0x93, rxBuff,8);
  StringPlusDataToHexaString(rxBuff,strBuff, 8);
  AsciUsrLog("Read from Rx queue:%s",strBuff);

  //Check for receive buffer errors
  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0x09, rxBuff,1);
  if(rxBuff[0]!=0)
    AsciErrLog("Receive Buffer Error, 0x%02X",rxBuff[0]);
*/

  AsciDbgLog("Init completed");

  return 0;
}

void AsciAdapterTask(void){


}



/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/
