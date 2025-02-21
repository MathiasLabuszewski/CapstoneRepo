/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "sys_app.h"
#include "subghz_phy_app.h"
#include "radio.h"
#include "stm32_timer.h"
#include "stm32_seq.h"
#include "utilities_def.h"
#include "adc_test.h"
#include "app_version.h"
#include "subghz_phy_version.h"
#include "utils.h"
#include <stdbool.h>


#define RX_TIMEOUT_VALUE              5000
#define TX_TIMEOUT_VALUE              1000

#define NUM_PAYLOADS_STORE 100
#define PAYLOAD_SIZE 8
#define CONF 0xCE

#define SWITCH_TIME 100

static RadioEvents_t RadioEvents;
bool TX_InProgress = false;
bool repeater = false;
static uint8_t BufferRx[PAYLOAD_SIZE];
static uint8_t BufferTx[PAYLOAD_SIZE];

static uint8_t conf;
static uint32_t deviceNum;
static uint32_t data;
static uint64_t payload;
static uint64_t BufferData[NUM_PAYLOADS_STORE];  // Store received payloads
static uint16_t bufferIndex = 0;                 // Track next free slot

uint16_t test = 0;

static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t LoraSnr_FskCfo);
static void OnTxDone(void);
static void OnTxTimeout(void);
static void OnRxTimeout(void);
static void OnRxError(void);
static void RX_Process(void);
static void TX_Process(void);
static void createPayload(void);
static bool isDuplicate(uint64_t payload);
static void storePayload(uint64_t payload);
uint16_t v1 = 0;
uint16_t v2 = 0;
uint16_t v3 = 0;
void SubghzApp_Init(void)
{
  /* Radio initialization */
  RadioEvents.RxDone = OnRxDone;
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxTimeout = OnRxTimeout;
  RadioEvents.RxError = OnRxError;

  Radio.Init(&RadioEvents);

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

  Radio.SetMaxPayloadLength(MODEM_LORA, PAYLOAD_SIZE);

  UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_RX), UTIL_SEQ_RFU, RX_Process);
  UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_TX), UTIL_SEQ_RFU, TX_Process);
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_RX), CFG_SEQ_Prio_RX);
}

static void OnTxDone(void)
{
    APP_LOG(TS_ON, VLEVEL_M, "TX Done: Successfully sent packet\n\r");
    TX_InProgress = false;
    HAL_GPIO_WritePin(LED_Port, LED_Pin, GPIO_PIN_SET);
    UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_RX), CFG_SEQ_Prio_RX);
}

static void OnRxDone(uint8_t *pay, uint16_t size, int16_t rssi, int8_t LoraSnr_FskCfo)
{
	payload = *(uint64_t*)pay;
	conf = (uint8_t)(payload >> 56);
    deviceNum = (uint32_t)(payload >> 24);
    data = ((uint32_t)(payload << 8)) >> 8;
	APP_LOG(TS_OFF, VLEVEL_L, "------------------------------------\n\r");
	APP_LOG(TS_OFF, VLEVEL_L, "RX Packet Successfully Received!\n\r");
	APP_LOG(TS_OFF, VLEVEL_L, "RssiValue=%d dBm, SnrValue=%ddB\n\r", rssi, LoraSnr_FskCfo);
	bool duplicate = isDuplicate(payload);
    if(conf == CONF && !duplicate){
		APP_LOG(TS_OFF, VLEVEL_L, "Confirmation - %02X\n\r", conf);
		APP_LOG(TS_OFF, VLEVEL_L, "Device Number - %08X\n\r", deviceNum);
		APP_LOG(TS_OFF, VLEVEL_L, "Data - %06X\n\r", data);
		APP_LOG(TS_OFF, VLEVEL_L, "Payload - %08X%08X\n\r", (uint32_t)(payload >> 32), (uint32_t)payload);
		APP_LOG(TS_OFF, VLEVEL_L, "Unique Packet! Repeating Now\n\r");
		repeater = true;
    }
    else if (duplicate){
	 	APP_LOG(TS_OFF, VLEVEL_L, "Duplicate packet detected! Not repeating.\n\r");
	 	repeater = false;
    }
    else{
    	APP_LOG(TS_OFF, VLEVEL_L, "Incorrect Confirmation Number\n\r");
    	repeater = false;
    }
	APP_LOG(TS_OFF, VLEVEL_L, "------------------------------------\n\r");
	memset(BufferRx, 0, PAYLOAD_SIZE);
	if(repeater)
		UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_TX), CFG_SEQ_Prio_TX);
	else
		UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_RX), CFG_SEQ_Prio_RX);
}

static void OnTxTimeout(void)
{
	APP_LOG(TS_OFF, VLEVEL_M, "TX Timeout: Retrying TX\n\r");
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
	HAL_GPIO_WritePin(LED_Port, LED_Pin, GPIO_PIN_RESET);
	APP_LOG(TS_ON, VLEVEL_M, "TX Start: Attempting to send payload\n\r");
	TX_InProgress = true;
	storePayload(payload);
	memcpy(BufferTx, &payload, PAYLOAD_SIZE);
    Radio.Send(BufferTx, PAYLOAD_SIZE);
}

static void createPayload(void){
	conf = CONF;
	deviceNum = UID_GetDeviceNumber();
	//data = (Radio.Random()) >> 8;
	adcRead(&data, &v2, &v3);
	payload = ((uint64_t)conf << 56)
                     | ((uint64_t)deviceNum << 24)
                     | ((uint64_t)data);

	APP_LOG(TS_OFF, VLEVEL_L, "------------------------------------\n\r");
	APP_LOG(TS_OFF, VLEVEL_L, "Payload Created:\n\r");
	APP_LOG(TS_OFF, VLEVEL_L, "Confirmation - %02X\n\r", conf);
	APP_LOG(TS_OFF, VLEVEL_L, "Device Number - %08X\n\r", deviceNum);
	APP_LOG(TS_OFF, VLEVEL_L, "Data - %06X\n\r", data);
	APP_LOG(TS_OFF, VLEVEL_L, "Payload - %08X%08X\n\r", (uint32_t)(payload >> 32), (uint32_t)payload);
    APP_LOG(TS_OFF, VLEVEL_L, "------------------------------------\n\r");
}


static bool isDuplicate(uint64_t payload)
{
    for (uint16_t i = 0; i < bufferIndex; i++) {
        if (BufferData[i] == payload)
            return true;
    }
    return false;
}

static void storePayload(uint64_t payload)
{
    if (bufferIndex < NUM_PAYLOADS_STORE) {
        BufferData[bufferIndex++] = payload;
    } else {
        APP_LOG(TS_OFF, VLEVEL_L, "Buffer full! Cannot store new payloads.\n\r");
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BUT_Pin && !TX_InProgress){
	  createPayload();
   	  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_TX), CFG_SEQ_Prio_TX);
  }
}
