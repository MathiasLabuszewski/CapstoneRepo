/*
 * Button_Driver.h
 *
 *  Created on: Feb 11, 2025
 *      Author: Jacob
 */

#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_



#endif /* BUTTON_DRIVER_H_ */
#include "GPIODriver.h"
#define BTN_PORT GPIOA
#define BTN_PIN 0
#define BTN_PRESSED 1
#define BTN_UNPRESSED 0
void BTN_Init();
bool BTN_Pressed();
void BTN_Interupt_Init();

