/*
 * adc_test.c
 *
 *  Created on: Nov 7, 2024
 *      Author: andrewhuynh1289
 */


#include "adc_test.h"

static ADC_HandleTypeDef hadc;

uint16_t t1 = 0;
uint16_t t2 = 0;
uint16_t t3 = 0;
void buttonInit(){
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Configure PA0 as input with pull-up/pull-down resistor
  GPIO_InitStruct.Pin = BUTTON_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;  // Interrupt on falling edge
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Enable and set EXTI line interrupt priority
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}


void adcInit(){


	GPIO_InitTypeDef fiveVoltRail = {0};

    // Enable the GPIO port clock for PB10
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Configure PB10 as output, push-pull, no pull-up/pull-down, and low speed
    fiveVoltRail.Pin = GPIO_PIN_10;
    fiveVoltRail.Mode = GPIO_MODE_OUTPUT_PP;
    fiveVoltRail.Pull = GPIO_NOPULL;
    fiveVoltRail.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &fiveVoltRail);

    // Set PB10 to HIGH to enable the 5V rail
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

	GPIO_InitTypeDef threeVoltRail = {0};

    // Enable the GPIO port clock for PB10
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure PB10 as output, push-pull, no pull-up/pull-down, and low speed
    threeVoltRail.Pin = GPIO_PIN_9;
    threeVoltRail.Mode = GPIO_MODE_OUTPUT_PP;
    threeVoltRail.Pull = GPIO_NOPULL;
    threeVoltRail.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOA, &threeVoltRail);

    // Set PB10 to HIGH to enable the 5V rail
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);


	hadc.Instance = ADC;
	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc.Init.LowPowerAutoWait = DISABLE;
	hadc.Init.LowPowerAutoPowerOff = DISABLE;
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.NbrOfConversion = 1;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.DMAContinuousRequests = DISABLE;
	hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_160CYCLES_5;
	hadc.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_160CYCLES_5;
	hadc.Init.OversamplingMode = DISABLE;
	hadc.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;

	//__HAL_RCC_ADC_CLK_ENABLE();
	//SET_BIT(ADC->CR, ADC_CR_ADVREGEN);
	//MODIFY_REG(ADC->CR, ADC_CR_BITS_PROPERTY_RS, ADC_CR_ADVREGEN);

	if (HAL_ADC_Init(&hadc) != HAL_OK)
	{
		while(1){};
	}

	  if (HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)
	  {
		/* Calibration Error */
		 while(1);
	  }

	HAL_ADC_Stop(&hadc);

	HAL_Delay(100);
	adcRead(&t1, &t2, &t3);

}

void selectChannel11(){

	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = ADC_CHANNEL_11;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_160CYCLES_5;

	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	{
		while(1){};
	}

}

void selectChannel2(){

	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_160CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	{
		while(1){};
	}

}

void selectChannel3(){

	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = ADC_CHANNEL_3;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_160CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	{
		while(1){};
	}

}

void adcRead(uint16_t *val1, uint16_t *val2, uint16_t *val3){


	selectChannel11();
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 100);
	*val1 = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);

	selectChannel2();
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 100);
	*val2 = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);

	selectChannel3();
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, 100);
	*val3 = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);

}





