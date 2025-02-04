/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "sys_app.h"
#include "subghz_phy_app.h"
#include "radio.h"
#include "stm32_timer.h"
#include "stm32_seq.h"
#include "utilities_def.h"
#include "app_version.h"
#include "subghz_phy_version.h"
#include "utils.h"
#include <stdbool.h>

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

bool TX_InProgress = false;
static uint64_t payloadSize;

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

static const uint8_t CONF = 206;

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
  APP_LOG(TS_OFF, VLEVEL_L, "==========================================\n\r");
  APP_LOG(TS_OFF, VLEVEL_L, "Capstone - BioSensor Solutions - LORA Test\n\r");
  APP_LOG(TS_OFF, VLEVEL_L, "LORA_BW=%d kHz\n\r", (1 << LORA_BANDWIDTH) * 125);
  APP_LOG(TS_OFF, VLEVEL_L, "LORA_SF=%d\n\r", LORA_SPREADING_FACTOR);
  APP_LOG(TS_OFF, VLEVEL_L, "==========================================\n\r");

  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, LORA_IQ_INVERSION_ON, TX_TIMEOUT_VALUE);

  Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                    0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

  Radio.SetMaxPayloadLength(MODEM_LORA, MAX_APP_BUFFER_SIZE);

  APP_LOG(TS_OFF, VLEVEL_M, "rand=%d\n\r", random_delay);

  UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_RX), UTIL_SEQ_RFU, RX_Process);
  UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_TX), UTIL_SEQ_RFU, TX_Process);
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_RX), CFG_SEQ_Prio_0);
}

static void OnTxDone(void)
{
    APP_LOG(TS_ON, VLEVEL_M, "TX Done: Successfully sent packet\n\r");
    APP_LOG(TS_OFF, VLEVEL_L, "------------------------------------\n\r");
    TX_InProgress = false;
    HAL_GPIO_WritePin(LED_Port, LED_Pin, GPIO_PIN_SET);
    UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_RX), CFG_SEQ_Prio_RX);
}

static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t LoraSnr_FskCfo)
{
	uint64_t *pay = (uint64_t*)payload;
	uint8_t conf = (uint8_t)(*pay >> 56);
    uint32_t deviceNum = (uint32_t)(*pay >> 24);
    uint32_t data = ((uint32_t)(*pay << 8)) >> 8;
	APP_LOG(TS_OFF, VLEVEL_L, "------------------------------------\n\r");
	APP_LOG(TS_OFF, VLEVEL_L, "RX Packet Successfully Received!\n\r");
	APP_LOG(TS_OFF, VLEVEL_L, "RssiValue=%d dBm, SnrValue=%ddB\n\r", rssi, LoraSnr_FskCfo);
    if(conf == CONF){
		APP_LOG(TS_OFF, VLEVEL_L, "Confirmation - %02X\n\r", conf);
		APP_LOG(TS_OFF, VLEVEL_L, "Device Number - %08X\n\r", deviceNum);
		APP_LOG(TS_OFF, VLEVEL_L, "Data - %06X\n\r", data);
		APP_LOG(TS_OFF, VLEVEL_L, "Payload - %08X%08X\n\r", (uint32_t)(*pay >> 32), (uint32_t)*pay);
    }
    else
    	APP_LOG(TS_OFF, VLEVEL_L, "Incorrect Confirmation Number\n\r");
	APP_LOG(TS_OFF, VLEVEL_L, "------------------------------------\n\r");
	memset(BufferRx, 0, MAX_APP_BUFFER_SIZE);
	UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_RX), CFG_SEQ_Prio_RX);
}

static void OnTxTimeout(void)
{
	APP_LOG(TS_ON, VLEVEL_M, "TX Timeout: Retrying TX\n\r");
	UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_TX), CFG_SEQ_Prio_TX);
}

static void OnRxTimeout(void)
{
	APP_LOG(TS_ON, VLEVEL_M, "RX Timeout: Retrying RX\n\r");
	UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_RX), CFG_SEQ_Prio_RX);
}

static void OnRxError(void)
{
	APP_LOG(TS_ON, VLEVEL_M, "RX Error: Retrying RX\n\r");
	UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_RX), CFG_SEQ_Prio_RX);
}

static void RX_Process(void)
{
	APP_LOG(TS_ON, VLEVEL_M, "RX Start\n\r");
	Radio.Rx(RX_TIMEOUT_VALUE);
}

static void TX_Process(void)
{
	if(!TX_InProgress){
		HAL_GPIO_WritePin(LED_Port, LED_Pin, GPIO_PIN_RESET);
		TX_InProgress = true;
		uint8_t conf = CONF;
		uint32_t deviceNum = UID_GetDeviceNumber();
		uint32_t data = (Radio.Random()) >> 8;
	    uint64_t payload = ((uint64_t)conf << 56)
	                     | ((uint64_t)deviceNum << 24)
	                     | ((uint64_t)data);
		APP_LOG(TS_OFF, VLEVEL_L, "------------------------------------\n\r");
		APP_LOG(TS_OFF, VLEVEL_L, "TX Start: Attempting to send payload\n\r");
		APP_LOG(TS_OFF, VLEVEL_L, "Confirmation - %02X\n\r", conf);
		APP_LOG(TS_OFF, VLEVEL_L, "Device Number - %08X\n\r", deviceNum);
		APP_LOG(TS_OFF, VLEVEL_L, "Data - %06X\n\r", data);
		APP_LOG(TS_OFF, VLEVEL_L, "Payload - %08X%08X\n\r", (uint32_t)(payload >> 32), (uint32_t)payload);
		payloadSize = sizeof(payload);
		memcpy(BufferTx, &payload, payloadSize);
	}
    Radio.Send(BufferTx, payloadSize);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BUT_Pin && !TX_InProgress)
    UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_TX), CFG_SEQ_Prio_TX);
}
