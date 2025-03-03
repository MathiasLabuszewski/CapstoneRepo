/*
 * LEDDriver.h
 *
 *  Created on: Jan 28, 2025
 *      Author: Jacob
 */

#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_



#endif /* LEDDRIVER_H_ */

#include "GPIODriver.h"

#define GREEN 1
#define RED 0

void LED_Init(uint8_t led);
void toggleLED(uint8_t led);
void turnOffLED(uint8_t led);
void turnOnLED(uint8_t led);
