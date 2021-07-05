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
#include "string.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
char strBuff[80];
uint32_t transaction;
/* Private function prototypes -----------------------------------------------*/
uint8_t PEC_calc(uint8_t *data, uint8_t size, uint8_t crc);
/* Private user code ---------------------------------------------------------*/




uint8_t AsciIoWriteSlaveReg(uint8_t slave, uint8_t regAddr, uint16_t value ){

  uint8_t status = ASCI_OK;
  uint8_t rxBuff[ASCI_RX_BUFFER_SIZE];

  //Egy irás SPI buszon keresztűl az UART-ra
  //SPI cmd | msg length | UART cmd(reg addres) | LSB|MSB | PEC | Alive
  uint8_t data[] = {SPI_CMD_WR_LD_Q, 0x06, UART_CMD_WRITEALL, regAddr, value & 0xFF, value >>8, 0x00, 0x00};
  uint8_t pec = PEC_calc(data + 2, 4, 0);
  data[6] = pec;
  memset(rxBuff, 0xCC, ASCI_RX_BUFFER_SIZE);
  if((status=AsciIoUartWriteRead(data,sizeof(data), rxBuff,sizeof(data) - 2))!=ASCI_OK)
  {
    AsciErrLog("AsciIoWriteSlaveReg.AsciIoUartWriteRead.Error:0x%02X", status);
    return status;
  }
  if(memcmp(data + 2, rxBuff, 5 ) !=0 ){
    //TODO: az alive countert nem ellenörzöm, ez jelzi hányan nyugtázták az üzenetet
    AsciErrLog("AsciIoWriteSlaveReg:ASCI_LOAD_VERIFY_ERROR");
    return ASCI_LOAD_VERIFY_ERROR;
  }
  return ASCI_OK;
}



uint8_t AsciIoSlaveReadReg(uint8_t slave, uint8_t regAddr, uint8_t *rx, uint8_t size){

  uint8_t status = ASCI_OK;
  //Egy olvasás az SPI  buszon keresztűl az UART-ról
  //SPI cmd | msg length + 2bájt * szlévek száma (ennyit kell olvasni)|Uart cmd(0x03)| Slave.reg.addr| Data Chec seed |PEC| Alive seed|
  uint8_t data[] = {SPI_CMD_WR_LD_Q, size, UART_CMD_READALL, regAddr,0x00, 0x00, 0x00};
  data[5]= PEC_calc(data + 2, 3, 0);
  if((status = AsciIoUartWriteRead(data,sizeof(data), rx, size))!= ASCI_OK){
    AsciErrLog("AsciIoSlaveReadReg.AsciIoUartWriteRead.Error:0x%02X", status);
    return status;
  }

  uint8_t rxPEC = rx[size - 2];
  uint8_t calcRxPEC =  PEC_calc(rx,size-2, 0);
  if(rxPEC != calcRxPEC){
    AsciErrLog("AsciIoSlaveReadReg rxPEC:0x%02X calcRxPEC:0x%02X", rxPEC, calcRxPEC);
    return ASCI_PEC_ERROR;
  }
  return ASCI_OK;
}


uint8_t AsciIoUartWriteRead(uint8_t *tx, uint8_t txSize, uint8_t *rx, uint8_t rxSize){

  uint8_t txBuff[ASCI_TX_BUFFER_SIZE];
  uint8_t rxBuff[ASCI_RX_BUFFER_SIZE];

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
  AsciIoWrite(tx, txSize);
  //StringPlusDataToHexaString(tx,strBuff, txSize);
  //AsciUsrLog("TR:%04ld-Load:%s",transaction,strBuff);

  //Verify content of the load queue
  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0xC1, rxBuff, txSize-1);

  if(memcmp(tx + 1, rxBuff, txSize-1)!=0){
    StringPlusDataToHexaString(rxBuff,strBuff, txSize-1);
    AsciErrLog("TR:%04ld-Verify:%s",transaction, strBuff);
    return ASCI_LOAD_VERIFY_ERROR;
  }

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
    memset(rx,0xCC, rxSize);
    AsciIoReadReg(SPI_CMD_RD_NXT_MSG, rx, rxSize);
    StringPlusDataToHexaString(rx,strBuff, rxSize);
    //AsciUsrLog("TR:%04ld-Read:%s", transaction, strBuff);
  }else{
    AsciErrLog("rx buffer is NULL");
  }

  //Check for receive buffer errors
  memset(rxBuff,0x00, sizeof(rxBuff));
  AsciIoReadReg(0x09, rxBuff,1);
  if(rxBuff[0]!=0  && rxBuff[0]!=0x02 )
    AsciErrLog("TR:%04ld-Receive Buffer Error, 0x%02X",transaction,rxBuff[0]);

  //AsciUsrLog("TR:%04ld--- Completed ---",transaction);

  return ASCI_OK;
}


void AsciIoPEC_CalcTest(void){
  DeviceDbgLog( "PEC for 0x03,0x12,0x00: 0x%02X (expected: 0xCB)",
                PEC_calc((uint8_t[]){0x03,0x12,0x00}, 3, 0x00));
  DeviceDbgLog( "PEC for 0x02,0x12,0xB1,0xB2: 0x%02X (expected: 0xC4)",
                PEC_calc((uint8_t[]){0x02,0x12,0xB1,0xB2}, 4, 0x00));

}

uint8_t PEC_calc(uint8_t *data, uint8_t size, uint8_t crc)
{
  uint8_t poly = 0xB2;
  for(uint8_t  byte =0; byte < size; byte++)
  {
    crc = crc ^ data[byte];
    for(uint8_t bit = 0; bit < 8; bit++)
    {
      if(crc & 0x01)
      {
        crc = ((crc/2)^poly);
      }
      else
      {
        crc = crc/2;
      }
      crc = crc & 0xFF;
    }
  }
  return crc;
}

uint8_t AsciIoWrite(uint8_t *writeBuffer, uint8_t size)
{
  uint8_t status = ASCI_OK;
  HAL_GPIO_WritePin(MAX_CS_GPIO_Port, MAX_CS_Pin, GPIO_PIN_RESET);
  if(HAL_SPI_Transmit(&hspi2, writeBuffer, size, 100)!= HAL_OK)
    status = ASCI_IO_ERROR;
  HAL_GPIO_WritePin(MAX_CS_GPIO_Port, MAX_CS_Pin, GPIO_PIN_SET);
  return status;
}


uint8_t AsciIoReadReg(uint8_t regAddr, uint8_t *rxBuffer, uint8_t size)
{
  uint8_t status = ASCI_OK;
  uint8_t txBuffer[size + 1];
  uint8_t rxTemp[size + 1];

  HAL_GPIO_WritePin(MAX_CS_GPIO_Port, MAX_CS_Pin, GPIO_PIN_RESET);
  memset(txBuffer,0xAA, size + 1);
  txBuffer[0] = regAddr;
  if(HAL_SPI_TransmitReceive(&hspi2, txBuffer, rxTemp, size + 1 , 100)!= HAL_OK)
    status = ASCI_IO_ERROR;
  memcpy(rxBuffer, rxTemp + 1, size);
  HAL_GPIO_WritePin(MAX_CS_GPIO_Port, MAX_CS_Pin, GPIO_PIN_SET);

  return status;
}


double ConvertMeasData(uint8_t *buffer){

  /*
   * AsciIoSlaveReadReg(0x00, SLAVE_REG_CELL1,rxBuff, SLAVE_RD_BUFFER_SIZE(slaveCnt));
   * AsciDbgLog("CELL1 Voltage:%f", (((rxBuff[3]<<8|rxBuff[2]))>>2) * 305.176E-6);
   * AsciDbgLog("CELL1c Voltage:%f", ((*(uint16_t*)rxBuff)>>2) * 305.176E-6);
   */
  double retval = 0;
  uint16_t value = *((uint16_t*)buffer);
  //sprintf(strBuff,"RX_Status:0x%02X",value);
  //AsciDbgLog(strBuff);
  value >>=2;
  retval = value * ADC_VREF_VOLT;
  return retval;
}

/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/
