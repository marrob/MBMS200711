/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "common.h"
#include "usbd_cdc_if.h"
#include "LiveLed.h"
#include "StringPlus.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MAX_ASCI_MODEL    0x15
#define MAX_ASCI_VERSION  0x17



#define MAX_ASCI_OK       0x00
#define MAX_ASCI_IO_ERROR 0x01

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN PV */
LiveLED_HnadleTypeDef hLiveLed;
uint32_t UartRxTimestamp;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */
void LiveLedOff(void);
void LiveLedOn(void);
void TestVcpTask(void);

uint8_t AsciGetModel();
void AsciWakeUp(void);
void AsciShutDown(void);
uint8_t AsciReadReg(uint8_t regAddr,  uint8_t *rxBuffer, uint8_t size);
uint8_t AsciWrite(uint8_t *writeBuffer, uint8_t size);
uint8_t AsciUartInitSeq(void);
uint8_t AsciUartInitSeq2(void);
void UsbPullUp(void);
void AsciTask(void);
uint8_t AsciReadRegU8(uint8_t regAddr,  uint8_t *byte);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /*** LiveLed ***/
  hLiveLed.LedOffFnPtr = &LiveLedOff;
  hLiveLed.LedOnFnPtr = &LiveLedOn;
  hLiveLed.HalfPeriodTimeMs = 500;
  LiveLedInit(&hLiveLed);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  DelayMs(500);

  UsbPullUp();


  printf(VT100_CLEARSCREEN);
  printf(VT100_CURSORHOME);
  printf(VT100_ATTR_RESET);

#ifdef DEBUG
  printf(VT100_ATTR_RED);
    DeviceUsrLog("This is a DEBUG version.");
  printf(VT100_ATTR_RESET);
#endif

  DeviceUsrLog("Manufacturer:%s, Name:%s, Version:%04X",DEVICE_MNF, DEVICE_NAME, DEVICE_FW);

AsciShutDown();
DelayMs(100);
AsciWakeUp();
DelayMs(100);

UartRxTimestamp = HAL_GetTick();

//  AsciGetModel();
 // AsciUartInitSeq();
  //AsciUartInitSeq2();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    LiveLedTask(&hLiveLed);
    CDC_Task_FS();
    //TestVcpTask();
    AsciTask();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LIVE_LED_GPIO_Port, LIVE_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MAX_CS_GPIO_Port, MAX_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, MAX_SHDN_Pin|USB_PULL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LIVE_LED_Pin */
  GPIO_InitStruct.Pin = LIVE_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LIVE_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MAX_CS_Pin */
  GPIO_InitStruct.Pin = MAX_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(MAX_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MAX_INT_Pin */
  GPIO_InitStruct.Pin = MAX_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MAX_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MAX_SHDN_Pin */
  GPIO_InitStruct.Pin = MAX_SHDN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(MAX_SHDN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PULL_Pin */
  GPIO_InitStruct.Pin = USB_PULL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(USB_PULL_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/* printf -------------------------------------------------------------------*/

//int _write(int file, char *ptr, int len)
//{
//  int i=0;
//  for(i=0 ; i<len ; i++)
//    ITM_SendChar((*ptr++));
//  return len;
//}
//
//int _write(int file, char *ptr, int len)
//{
//  HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 100);
//  return len;
//}

int _write(int file, char *ptr, int len)
{
  CDC_Transmit_FS((uint8_t*)ptr, len);
  return len;
}

void TestVcpTask(void)
{
  static int32_t timestamp;
  static uint64_t counter = 0;
  if(HAL_GetTick() - timestamp >= 1000)
  {
    timestamp = HAL_GetTick();

    char test_sentence[80] = {"Hello World"};
    sprintf(test_sentence, "%s %lld\r\n", test_sentence,counter++);

    strcpy(UsbdUart.TxLine,test_sentence);
    printf(test_sentence);
    UsbdUart.TxCounter++;

    if(UsbdUart.RxRequest)
    {
      printf("%s\r\n",UsbdUart.RxLine);
      UsbdUart.RxRequest = 0;
      UsbdUart.TxCounter++;
    }
  }
}
/* MAX ASCI -----------------------------------------------------------------*/

uint8_t AsciGetModel(void)
{

  uint8_t modelNumber[] = {0}; /*Defult is 0x84*/
  AsciReadReg(MAX_ASCI_MODEL, modelNumber, sizeof(modelNumber));
  DeviceDbgLog("Model Number 0x%02X", modelNumber[0]);
  uint8_t versionNumber[] = {0}; /*Defult is 0x12*/
  AsciReadReg(MAX_ASCI_VERSION, versionNumber, sizeof(versionNumber));
  DeviceDbgLog("Version Number 0x%02X", versionNumber[0]);
  return 0;
}

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


uint8_t AsciUartInitSeq(void)
{

  /*Enable Keep-Alive mode (prior to the UART slave wake-up to prevent shutdown)*/
  //Write Confguration 3 register
  //Set keep-alive period to 160µs
  uint8_t tr1[] = {0x10, 0x05};
  AsciWrite(tr1, sizeof(tr1));



#if sdfasf
  uint8_t trX1[] = {0x00};
  AsciReadReg(0x09,trX1, sizeof(trX1));

  /*Enable Rx Interrupt ﬂags for RX_Error and RX_Overﬂow*/
  //Write RX_Interrupt_Enable register
  //Set the RX_Error_INT_Enable and RX_Overﬂow_INT_Enable bits
  uint8_t tr2[] = {0x04, 0x88};
  AsciWrite(tr2, sizeof(tr2));

  /*Clear receive buﬀer*/
  uint8_t tr3[] = {0xE0};
  AsciWrite(tr3, sizeof(tr3));

  /*Wake-up UART slave devices (transmit preambles)*/
  //Write Confguration 2 register
  //Enable Transmit Preambles mode
  uint8_t tr4[] = {0x0E, 0x30};
  AsciWrite(tr4, sizeof(tr4));


  uint8_t trX2[] = {0x00};
  AsciReadReg(0x09, trX2, sizeof(trX2));

  /*Wait for all UART slave devices to wake up (poll RX_Busy_Status bit)*/
  //Read RX_Status register (RX_Busy_Status and RX_Empty_Status should be true)
  //If RX_Status = 21h, continue. Otherwise, repeat transaction until true or timeout.
  uint8_t tr5[1];
  do
  {
    AsciReadReg(0x01, tr5, sizeof(tr5));
    DelayMs(20);
  }while(tr5[0]!=0x21);

  /*End of UART slave device wake-up period*/
  //Write Confguration 2 register
  //Disable Transmit Preambles mode
  uint8_t tr6[] = {0x0E, 0x10};
  AsciWrite(tr6, sizeof(tr6));

  /*Wait for null message to be received (poll RX_Empty_Status bit)*/
  //Read RX_Status register
  uint8_t tr7[] = {0x01};
  AsciWrite(tr7, sizeof(tr7));
#endif

  /*Clear transmit buﬀer*/
  uint8_t tr8[] = {0x20};
  AsciWrite(tr8, sizeof(tr8));

  /*Clear receive buﬀer*/
  uint8_t tr9[] = {0xE0};
  AsciWrite(tr9, sizeof(tr9));

  /*Load the HELLOALL command sequence into the load queue*/
  //WR_LD_Q SPI command byte (write the load queue)
  //Message length
  //HELLOALL command byte
  //Register address (0x00)
  //Initialization address of HELLOALL
  uint8_t tr10[] = {0xC0,0x03,0x57,0x00,0x0};
  AsciWrite(tr10, sizeof(tr10));

  /*Verify contents of the load queue*/
  //Verify contents of the load queue
  uint8_t tr11[4];
  AsciReadReg(0xC1,tr11,sizeof(tr11));

  /*Transmit HELLOALL sequence*/
  //WR_NXT_LD_Q SPI command byte (write the next load queue)
  uint8_t tr12[4] = {0xB0};
  AsciWrite(tr12, sizeof(tr12));


  /*Poll RX_Stop_Status bit*/
  //Read RX_Status register
  //If RX_Status[1] is true, continue. If false, then repeat transaction until true.

//    uint8_t tr13[1];
//    do
//    {
//      AsciReadReg(0x01, tr13, sizeof(tr13));
//
//    }while(tr13[0]!=0x12);

  /*Service receive buﬀer. Read the HELLOALL message that propagated through the daisy-chain and was
  returned back to the ASCI. The host should verify the device count.*/
  //RD_NXT_MSG SPI transaction
  //Sent command byte (HELLOALL)
  //Sent address = 00h
  //Returned address = 02h
  uint8_t tr14[3];
  AsciReadReg(0x93, tr14, sizeof(tr14));

  /*Check for receive buﬀer errors*/
  //Read RX_Interrupt_Flags register
  uint8_t tr15[1];
  AsciReadReg(0x09, tr15, sizeof(tr15));



    return 0;
}


uint8_t AsciWrite(uint8_t *writeBuffer, uint8_t size)
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


uint8_t AsciReadRegU8(uint8_t regAddr,  uint8_t *byte)
{
  return AsciReadReg(regAddr,  byte, 1 );

}

uint8_t AsciReadReg(uint8_t regAddr,  uint8_t *rxBuffer, uint8_t size)
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





void AsciWakeUp(void)
{
  HAL_GPIO_WritePin(MAX_SHDN_GPIO_Port, MAX_SHDN_Pin, GPIO_PIN_SET);
}

void AsciShutDown(void)
{
  HAL_GPIO_WritePin(MAX_SHDN_GPIO_Port, MAX_SHDN_Pin, GPIO_PIN_RESET);
}

/* LEDs ---------------------------------------------------------------------*/
void LiveLedOn(void)
{
  HAL_GPIO_WritePin(LIVE_LED_GPIO_Port, LIVE_LED_Pin, GPIO_PIN_SET);
}

void LiveLedOff(void)
{
  HAL_GPIO_WritePin(LIVE_LED_GPIO_Port, LIVE_LED_Pin, GPIO_PIN_RESET);
}
/* USB ---------------------------------------------------------------------*/
void UsbPullUp(void)
{
  HAL_GPIO_WritePin(USB_PULL_GPIO_Port, USB_PULL_Pin, GPIO_PIN_SET);
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
