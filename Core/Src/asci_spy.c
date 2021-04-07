/*
 * asci_spy.c
 *
 *  Created on: Apr 3, 2021
 *      Author: Margit Robert
 */

/* Includes ------------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
# ifdef x
uint8_t AsciUartInitSeq2(void)
{

  uint8_t rxBuffer[100];
  uint8_t txBuffer[100];

  /* TX_Interrupt_Flags.POR_Flag 0x80*/
  AsciReadReg(0x0B,rxBuffer,1);
  /*MODEL REGISTER.First two digits of the Model Number (84h)*/
  AsciReadReg(0x15,rxBuffer,1);
  /*VERSION REGISTER Last digit of the Model Number (1h)/Mask revision (2)*/
  AsciReadReg(0x17,rxBuffer,1);
  /*FMEA REGISTER:0x00*/
  AsciReadReg(0x13,rxBuffer,1);
  /*RX_INTERRUPT_FLAG REGISTER: Write 0*/
  memcpy(txBuffer, (uint8_t[]){0x08,0x00}, 2);
  AsciWrite(txBuffer, 2);

  /*RX_INTERRUPT_FLAG REGISTER: Write 0*/
  memcpy(txBuffer, (uint8_t[]){0x08,0x00}, 2);
  AsciWrite(txBuffer, 2);


  /*RX_INTERRUPT_ENABLE REGISTER: RX_Stop_INT_Enable */
  memcpy(txBuffer, (uint8_t[]){0x04,0x02}, 2);
  AsciWrite(txBuffer, 2);
  /*TX_INTERRUPT_ENABLE REGISTER: Write 0*/
  memcpy(txBuffer, (uint8_t[]){0x06,0x00}, 2);
  AsciWrite(txBuffer, 2);
  /*CONFIGURATION_1 REGISTER:2Mbps*/
  memcpy(txBuffer, (uint8_t[]){0x0C,0x60}, 2);
  AsciWrite(txBuffer, 2);
  /*CONFIGURATION_2 REGISTER:TransmitQueueMode*/
  memcpy(txBuffer, (uint8_t[]){0x0E,0x10}, 2);
  AsciWrite(txBuffer, 2);
  /*CONFIGURATION_3 REGISTER:DOUT_Enable | Keep_Alive 10us*/
  memcpy(txBuffer, (uint8_t[]){0x10,0x05}, 2);
  AsciWrite(txBuffer, 2);


  memcpy(txBuffer, (uint8_t[]){0x30}, 1);
  AsciWrite(txBuffer, 1);

  memcpy(txBuffer, (uint8_t[]){0xE0}, 1);
  AsciWrite(txBuffer, 1);

  memcpy(txBuffer, (uint8_t[]){0x93}, 1);
  AsciWrite(txBuffer, 1);
//
//  //DelayMs(1);

  /*CONFIGURATION_2 REGISTER:TransmitQueueMode*/
  memcpy(txBuffer, (uint8_t[]){0x0E,0x10}, 2);
  AsciWrite(txBuffer, 2);
  /*CONFIGURATION_2 REGISTER:TransmitQueueMode & Transmit Preambles Mode*/
  memcpy(txBuffer, (uint8_t[]){0x0E,0x30}, 2);
  AsciWrite(txBuffer, 2);





  /*RX_STATUS REGISTER: RX_Empty_Status | RX_Busy_Status*/
  /*Wait for all UART slave devices to wake up (poll RX_Busy_Status bit)*/
  //Read RX_Status register (RX_Busy_Status and RX_Empty_Status should be true)
  //If RX_Status = 21h, continue. Otherwise, repeat transaction until true or timeout.
  uint32_t timestamp = HAL_GetTick();

  do
  {
    AsciReadReg(0x01, rxBuffer, 1);
    if(HAL_GetTick() - timestamp > 200)
    {
      uint8_t bufi[100];
      AsciReadReg(0x93,bufi, 6);

      memcpy(txBuffer, (uint8_t[]){0x30}, 1);
      AsciWrite(txBuffer, 1);

      memcpy(txBuffer, (uint8_t[]){0xE0}, 1);
      AsciWrite(txBuffer, 1);


      memcpy(txBuffer, (uint8_t[]){0xC0,0x06,0x04,0x1B,0x00,0x80,0xA2,0x00}, 8);
      AsciWrite(txBuffer, 8);

      AsciWrite(txBuffer, 1);
      memcpy(txBuffer, (uint8_t[]){0xB0}, 1);
      timestamp = HAL_GetTick();
    }



  }while(rxBuffer[0]!=0x21);

//
//
//  AsciReadReg(0x93,rxBuffer, 6);
//  memcpy(txBuffer, (uint8_t[]){0x30}, 1);
//  AsciWrite(txBuffer, 1);
//
//  memcpy(txBuffer, (uint8_t[]){0xE0}, 1);
//  AsciWrite(txBuffer, 1);
//
//  timestamp = HAL_GetTick();
//  do
//  {
//    AsciReadReg(0x01, rxBuffer, 1);
////    if(HAL_GetTick() - timestamp > 1000)
////      break;
//  }while(rxBuffer[0]!=0x21);

  return 0;
}

#define RX_STATUS_READ_REG  0x01

#define RX_STATUS_EMPTY     1
#define RX_STATUS_STOP      2
#define RX_STATUS_FULL      4
#define RX_STATUS_OVER      8
#define RX_STATUS_IDLE      16
#define RX_STATUS_BUSY      32
#define RX_STATUS_ERROR     128

#define RX_SPACE_RD_REG     0x1B

#define RX_NXT_MSG_REG      0x93
#define RX_NXT_PTR_REG      0x9B

#define RX_READ_REG         0x91
#define RX_READ_PTR_REG     0x97

char String[200];
char String2[200];

void AsciInit(void)
{
  uint8_t rxBuffer[100];
  uint8_t txBuffer[100];

  /*MODEL REGISTER.First two digits of the Model Number (84h)*/
  AsciReadReg(0x15,rxBuffer,1);
  /*VERSION REGISTER Last digit of the Model Number (1h)/Mask revision (2)*/
  AsciReadReg(0x17,rxBuffer,1);
  /*FMEA REGISTER:0x00*/
  AsciReadReg(0x13,rxBuffer,1);

  /*Clear receive buﬀer*/
  memcpy(txBuffer, (uint8_t[]){0xE0}, 1);
  AsciWrite(txBuffer, 1);
}

void AsciTaskCirciualMethod(void)
{
  uint8_t msgBuffer[62];
  uint8_t rxStatus;
  uint8_t ptrBefore;
  uint8_t ptrAfter;
  uint8_t length;



  AsciReadRegU8(RX_STATUS_READ_REG, &rxStatus);

  if(rxStatus & RX_STATUS_STOP )
  {
     AsciReadRegU8(RX_READ_PTR_REG, &ptrBefore);
     AsciReadReg(RX_NXT_MSG_REG, msgBuffer, sizeof(msgBuffer));
     AsciReadRegU8(RX_READ_PTR_REG, &ptrAfter);

     if((ptrAfter - ptrBefore) < 0)
     {
       length = ptrAfter + (64-ptrBefore);
     }
     else
     {
       length = ptrAfter - ptrBefore;
     }

     sprintf(String2,"ptrBefore:%02d, ptrAfter:%02d, value: %s ",ptrBefore,  ptrAfter, StringPlusDataToHexaString(msgBuffer, String, length));
     DeviceDbgLog(String2);
  }

  if(rxStatus & RX_STATUS_OVER)
  {
    DeviceErrLog("RX_STATUS_OVER");
  }
  if(rxStatus & RX_STATUS_ERROR)
  {
    DeviceErrLog("RX_STATUS_ERROR");
  }

}
void AsciTask(void)
{
  uint8_t msgBuffer[62];
  uint8_t rxStatus;
  uint8_t ptrBefore;
  uint8_t ptrAfter;


  AsciReadRegU8(RX_STATUS_READ_REG, &rxStatus);

  if(rxStatus & RX_STATUS_STOP )
  {

    AsciReadRegU8(RX_READ_PTR_REG, &ptrBefore);
    AsciReadReg(RX_NXT_MSG_REG, msgBuffer, sizeof(msgBuffer));
    AsciReadRegU8(RX_READ_PTR_REG, &ptrAfter);

    uint32_t reltiveTime = HAL_GetTick() - UartRxTimestamp;
    UartRxTimestamp = HAL_GetTick();
    //sprintf(String2,"ptrBefore:%02d, ptrAfter:%02d, value: %s ",ptrBefore,  ptrAfter, StringPlusDataToHexaString(msgBuffer, String, ptrAfter - ptrBefore));
    sprintf(String2,"R+%06ldms:  %s", reltiveTime, StringPlusDataToHexaString(msgBuffer, String, ptrAfter - ptrBefore));
    DeviceDbgLog(String2);

    /*Clear Buffer*/
    AsciWrite((uint8_t[]){0xE0}, 1);
  }

  if(rxStatus & RX_STATUS_OVER)
  {
    DeviceErrLog("RX_STATUS_OVER");
  }
  if(rxStatus & RX_STATUS_ERROR)
  {
    DeviceErrLog("RX_STATUS_ERROR");
  }

}
#endif

/************************ (C) COPYRIGHT KonvolucioBt ***********END OF FILE****/
