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
  uint8_t txBuff[28];
  uint8_t rxBuff[62];
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
uint8_t AsciAdapterInit(void)
{
  uint8_t txBuff[ASCI_TX_BUFFER_SIZE];
  uint8_t rxBuff[ASCI_RX_BUFFER_SIZE];
  uint8_t slaveCnt = 1;

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

  //Disable Transmit Preambles mode
  memcpy(txBuff, (uint8_t[]){0x0E,0x10}, 2);
  AsciIoWrite(txBuff, 2);

  //Enable Keep-Alive mode (prior to the UART slave wake-up to prevent shutdown)
  memcpy(txBuff, (uint8_t[]){0x10,0x05}, 2);
  AsciIoWrite(txBuff, 2);

  //A 0x30-nak nincs dokumentálva a funkciója
  //Ez sztm 0x20-nak kellene lennie
  //Clear transmit buffer
  memcpy(txBuff, (uint8_t[]){0x20}, 1);
  AsciIoWrite(txBuff, 1);

  //Clear receive buffer
  memcpy(txBuff, (uint8_t[]){0xE0}, 1);
  AsciIoWrite(txBuff, 1);


  memcpy(txBuff, (uint8_t[]){0x93}, 1);
  AsciIoWrite(txBuff, 1);

  //Ha van slave a buszon, akkor az RX_Busy_Status bit időközönként 1-be áll (0x21),
  //ha nincs akkor elveszett a busz.
  //Poll Rx_Busy_Status bit
  int16_t i=0;
  do
  {
    AsciIoReadReg(0x01, rxBuff, 1);
    sprintf(strBuff,"%03d Read RX_Status:0x%02X",i,rxBuff[0]);
    AsciDbgLog(strBuff);
    i++;
  }while(rxBuff[0]!=0x21 && i < ASCI_RX_TIMEOUT_CYCLE);

  if(i >= ASCI_RX_TIMEOUT_CYCLE)
  {
    //Első futás kor amig nincs hurok, ez itt hibára fog futni
    AsciErrLog("Timeout.");
  }
  AsciDbgLog("Master init completed");


 //HELLOALL - Eslő inditás után ez itt timoeutra fog futni
 AsciIoUartWriteRead((uint8_t[]){0xC0,0x03,0x57,0x00,0x00}, 5, rxBuff,10);


 //meg kell mondani hová teszem hurkot, ennélkül nem válaszol
 //első inditás után timeoutra fog futni
 AsciIoUartWriteRead((uint8_t[]){0xC0,0x07,0x04,0x1B,0x00,0x80,0xA2,0x00,0x00},9, rxBuff, 10);

 //HELLO
 //Erre már válaszol mivel kiépült a hurok
 AsciIoUartWriteRead((uint8_t[]){0xC0,0x03,0x57,0x00,0x00}, 5, rxBuff,10);




 //AsciIoUartWriteRead((uint8_t[]){0xC0,0x06,0x02,0x12,0xB1, 0xB2, 0xC4, 0x00}, 8, rxBuff,10);
 AsciIoWriteSlaveReg(0x00, 0x12, 0xB3FF);


 //AsciIoUartWriteRead((uint8_t[]){0xC0,0x05,0x03,0x12,0x00, 0xB2, 0xCB}, 7, rxBuff,20);
 AsciIoSlaveReadReg(0x00, 0x12,rxBuff, SLAVE_RD_BUFFER_SIZE(slaveCnt));

 AsciIoSlaveReadReg(0x00, 0x20,rxBuff, SLAVE_RD_BUFFER_SIZE(slaveCnt));


 AsciIoSlaveReadReg(0x00, 0x00,rxBuff, SLAVE_RD_BUFFER_SIZE(slaveCnt));
 AsciDbgLog("Slave VERSION Reg: 0x%04X", rxBuff[3]<<8|rxBuff[2]);

 AsciIoSlaveReadReg(0x00, 0x01,rxBuff, SLAVE_RD_BUFFER_SIZE(slaveCnt));
 AsciDbgLog("Slave ADDRESS Reg: 0x%04X", rxBuff[3]<<8|rxBuff[2]);

 AsciIoSlaveReadReg(0x00, 0x02,rxBuff, SLAVE_RD_BUFFER_SIZE(slaveCnt));
 AsciDbgLog("Slave STATUS Reg: 0x%04X", rxBuff[3]<<8|rxBuff[2]);

  return 0;
}

void AsciAdapterTask(void){


   static uint16_t i=0;
   AsciIoReadReg(0x01, rxBuff, 1);
   if(rxBuff[0]==0x21)
   {
     //Ha van slave a buszon és zárt a hurok, akkor az RX_Busy_Status bit időközönként 1-be áll (0x21),
     //ha nincs akkor elveszett a busz.
     //sprintf(strBuff,"%03d Read RX_Status:0x%02X", i++,rxBuff[0]);
     //AsciDbgLog(strBuff);
   }
   else if(rxBuff[0]==0x12)
   {
     sprintf(strBuff,"%03d Read RX_Status:0x%02X", i++,rxBuff[0]);
     AsciDbgLog(strBuff);

     //Receive
     memset(rxBuff,0x00, sizeof(rxBuff));
     AsciIoReadReg(0x93, rxBuff,10);
     StringPlusDataToHexaString(rxBuff,strBuff, 10);
     AsciUsrLog("Read from Rx queue:%s",strBuff);
   }

}



/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/
