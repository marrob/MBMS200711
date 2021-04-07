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
#include "main.h"
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
  DeviceDbgLog(strBuff);

  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0x15,rxBuff,1);
  sprintf(strBuff,"Read Model Register:0x%02X",rxBuff[0]);
  DeviceDbgLog(strBuff);

  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0x17,rxBuff,1);
  sprintf(strBuff,"Read Version Register:0x%02X",rxBuff[0]);
  DeviceDbgLog(strBuff);

  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0x13,rxBuff,1);
  sprintf(strBuff,"Read FEMA Register:0x%02X",rxBuff[0]);
  DeviceDbgLog(strBuff);

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

  int16_t i=0;
  do
  {
    AsciIoReadReg(0x01, rxBuff, 1);
    sprintf(strBuff,"%03d Read RX_Status:0x%02X",i,rxBuff[0]);
    DeviceDbgLog(strBuff);
    i++;
}while(rxBuff[0]!=0x21 && i < 100);



  memcpy(txBuff, (uint8_t[]){0x30}, 1);
  AsciIoWrite(txBuff, 1);

  memcpy(txBuff, (uint8_t[]){0xE0}, 1);
  AsciIoWrite(txBuff, 1);

  memcpy(txBuff, (uint8_t[]){0xC0,0x03,0x57,0x00,0x00}, 5);
  AsciIoWrite(txBuff, 5);

  memcpy(txBuff, (uint8_t[]){0xB0}, 1);
  AsciIoWrite(txBuff, 1);

  memcpy(txBuff, (uint8_t[]){0x30}, 1);
  AsciIoWrite(txBuff, 1);

  memcpy(txBuff, (uint8_t[]){0xE0}, 1);
  AsciIoWrite(txBuff, 1);

  memcpy(txBuff, (uint8_t[]){0xC0,0x07,0x04,0x1B,0x00,0x80,0xA2,0x00,0x00}, 9);
  AsciIoWrite(txBuff, 9);

  memcpy(txBuff, (uint8_t[]){0xB0}, 1);
  AsciIoWrite(txBuff, 1);

  i=0;
  do
  {
    AsciIoReadReg(0x01, rxBuff, 1);
    sprintf(strBuff,"%03d Read RX_Status:0x%02X",i,rxBuff[0]);
    DeviceDbgLog(strBuff);
    i++;
    DelayMs(10);
}while(rxBuff[0]!=0x21 && i < 1000);



  return 0;
}
/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/
