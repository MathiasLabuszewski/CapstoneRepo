/*
 * Button_Driver.c
 *
 *  Created on: Feb 11, 2025
 *      Author: Jacob
 */

#include "Button_Driver.h"

GPIO_PinConfig_t BTN = {GPIO_PIN_0,GPIO_MODE_INPUT,GPIO_OPTYPE_PUSHPULL,GPIO_SPEED_MEDIUM,GPIO_PUPD_NONE,NO_INTERUPT};

void BTN_Init() {
	GPIO_Init(GPIOA,BTN); //btn is on GPIOA clock
}

bool BTN_Pressed() {
	if(GPIO_ReadOutputPin(GPIOA,BTN.PinNumber) == BTN_PRESSED) {
		return true;
	}
	else {
		return false;
	}

}
void BTN_Interupt_Init() {
	BTN.PinIntMode = FALLING_RISING_INTERUPT;
	GPIO_Init(GPIOA,BTN);
	GPIO_WriteGPIOInterupt(EXTI0_IRQ_NUMBER, ACTIVE);
}
