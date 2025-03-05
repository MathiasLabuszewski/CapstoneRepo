/*
 * adc_test.h
 *
 *  Created on: Nov 7, 2024
 *      Author: andrewhuynh1289
 */

#ifndef INC_ADC_TEST_H_
#define INC_ADC_TEST_H_

#include "stm32wlxx_hal.h"
#include <stdint.h>
#include <stdio.h>

#define BUTTON_PIN GPIO_PIN_0

//channel 3  PB4 (A4 pin) on wioe5
//channel 11 PA15 (SDA of I2C) on wioe5
//channel 2  PB3 (A3 pin) on wioe5

void adcInit();
void adcRead();
void selectChannel11();
void selectChannel3();
void selectChannel2();
void buttonInit();

#endif /* INC_ADC_TEST_H_ */
