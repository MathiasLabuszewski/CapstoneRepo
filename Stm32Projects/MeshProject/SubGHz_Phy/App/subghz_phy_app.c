/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    subghz_phy_app.c
  * @author  MCD Application Team
  * @brief   Application of the SubGHz_Phy Middleware
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "sys_app.h"
#include "subghz_phy_app.h"
#include "radio.h"

/* USER CODE BEGIN Includes */
#include "stm32_timer.h"
#include "stm32_seq.h"
#include "utilities_def.h"
#include "app_version.h"
#include "subghz_phy_version.h"
/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Configurations */
/*Timeout*/
#define RX_TIMEOUT_VALUE              5000
#define TX_TIMEOUT_VALUE              1000

/*Size of the payload to be sent*/
/* Size must be greater of equal the PING and PONG*/
#define MAX_APP_BUFFER_SIZE          255
#if (PAYLOAD_LEN > MAX_APP_BUFFER_SIZE)
#error PAYLOAD_LEN must be less or equal than MAX_APP_BUFFER_SIZE
#endif /* (PAYLOAD_LEN > MAX_APP_BUFFER_SIZE) */
/* wait for remote to be in Rx, before sending a Tx frame*/
#define RX_TIME_MARGIN                200
/* Afc bandwidth in Hz */
#define FSK_AFC_BANDWIDTH             83333
/* LED blink Period*/
#define LED_PERIOD_MS                 200

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* Radio events function pointer */
static RadioEvents_t RadioEvents;


/* App Rx Buffer*/
static uint8_t BufferRx[MAX_APP_BUFFER_SIZE];
/* App Tx Buffer*/
static uint8_t BufferTx[MAX_APP_BUFFER_SIZE];
/* Last  Received Buffer Size*/
uint16_t RxBufferSize = 0;
/* random delay to make sure 2 devices will sync*/
/* the closest the random delays are, the longer it will
   take for the devices to sync when started simultaneously*/
static int32_t random_delay;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t LoraSnr_FskCfo);
static void OnTxDone(void);
static void OnTxTimeout(void);
static void OnRxTimeout(void);
static void OnRxError(void);
static void RX_Process(void);
static void TX_Process(void);

void SubghzApp_Init(void)
{
  /* Radio initialization */
  RadioEvents.RxDone = OnRxDone;
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxTimeout = OnRxTimeout;
  RadioEvents.RxError = OnRxError;

  Radio.Init(&RadioEvents);

  random_delay = (Radio.Random()) >> 22; /*10bits random e.g. from 0 to 1023 ms*/

  /* Radio Set frequency */
  Radio.SetChannel(RF_FREQUENCY);

  /* Radio configuration */
  APP_LOG(TS_OFF, VLEVEL_M, "Capstone - BioSensor Solutions - LORA Test\n\r");
  APP_LOG(TS_OFF, VLEVEL_M, "LORA_BW=%d kHz\n\r", (1 << LORA_BANDWIDTH) * 125);
  APP_LOG(TS_OFF, VLEVEL_M, "LORA_SF=%d\n\r", LORA_SPREADING_FACTOR);

  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, LORA_IQ_INVERSION_ON, TX_TIMEOUT_VALUE);

  Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                    0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

  Radio.SetMaxPayloadLength(MODEM_LORA, MAX_APP_BUFFER_SIZE);

  APP_LOG(TS_OFF, VLEVEL_L, "rand=%d\n\r", random_delay);

  UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_RX), UTIL_SEQ_RFU, RX_Process);
  UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_TX), UTIL_SEQ_RFU, TX_Process);
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_RX), CFG_SEQ_Prio_0);
}

static void OnTxDone(void)
{
  APP_LOG(TS_ON, VLEVEL_L, "TX Done: Successfully sent packet\n\r");
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_RX), CFG_SEQ_Prio_RX);
}

static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t LoraSnr_FskCfo)
{
	uint32_t *pay = (uint32_t)payload;
	APP_LOG(TS_ON, VLEVEL_L, "RX Packet Successfully Received! Payload - %d\n\r", *pay);
	APP_LOG(TS_ON, VLEVEL_L, "RssiValue=%d dBm, SnrValue=%ddB\n\r", rssi, LoraSnr_FskCfo);
	memset(BufferRx, 0, MAX_APP_BUFFER_SIZE);
	UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_RX), CFG_SEQ_Prio_RX);
}

static void OnTxTimeout(void)
{
	APP_LOG(TS_ON, VLEVEL_L, "TX Timeout: Retrying TX\n\r");
	UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_TX), CFG_SEQ_Prio_TX);
}

static void OnRxTimeout(void)
{
	APP_LOG(TS_ON, VLEVEL_L, "RX Timeout: Retrying RX\n\r");
	UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_RX), CFG_SEQ_Prio_RX);
}

static void OnRxError(void)
{
	APP_LOG(TS_ON, VLEVEL_L, "RX Error: Retrying RX\n\r");
	UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_RX), CFG_SEQ_Prio_RX);
}

static void RX_Process(void)
{
	APP_LOG(TS_ON, VLEVEL_L, "RX Start\n\r");
	Radio.Rx(RX_TIMEOUT_VALUE);
}

static void TX_Process(void)
{
	HAL_GPIO_WritePin(LED_Port, LED_Pin, GPIO_PIN_SET);
    uint32_t payload = (Radio.Random()) >> 22;
    APP_LOG(TS_ON, VLEVEL_L, "TX Start: Attempting to send payload - %d\n\r", payload);
	HAL_Delay(Radio.GetWakeupTime() + RX_TIME_MARGIN);
    memcpy(BufferTx, &payload, sizeof(payload));
    Radio.Send(BufferTx, sizeof(payload));
    HAL_GPIO_WritePin(LED_Port, LED_Pin, GPIO_PIN_RESET);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BUT_Pin)
  {
    APP_LOG(TS_ON, VLEVEL_H, "Button Pressed: Adding TX Task to Sequencer\n\r");
    UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_TX), CFG_SEQ_Prio_TX);
  }
}
