/*
 * LEDDriver.c
 *
 *  Created on: Jan 28, 2025
 *      Author: Jacob
 */


#include "LEDDriver.h"
/*
 *
 * typedef struct {
	uint8_t PinNumber;
	uint8_t PinMode;
	uint8_t OPType;
	uint8_t PinSpeed;
	uint8_t PinPuPdControl;


}GPIO_PinConfig_t;

 */
GPIO_PinConfig_t RED_LED = {GPIO_PIN_14,GPIO_MODE_OUTPUT,GPIO_OPTYPE_PUSHPULL,GPIO_SPEED_MEDIUM,GPIO_PUPD_NONE};
GPIO_PinConfig_t GREEN_LED = {GPIO_PIN_13,GPIO_MODE_OUTPUT,GPIO_OPTYPE_PUSHPULL,GPIO_SPEED_MEDIUM,GPIO_PUPD_NONE};
void LED_Init(uint8_t led) {

	switch(led) {
					case 0:

						GPIO_Init(GPIOG,RED_LED);
						break;
					case 1:

						GPIO_Init(GPIOG,GREEN_LED);
						break;
	}

}
void toggleLED(uint8_t led) {

	switch(led) {
		case 0: GPIO_ToggleOutputPin(GPIOG,RED_LED.PinNumber);
		break;

		case 1: GPIO_ToggleOutputPin(GPIOG,GREEN_LED.PinNumber);
		break;
	}
}
void turnOffLED(uint8_t led) {
	switch(led) {
		case 0: GPIO_WriteToOutputPin(GPIOG,RED_LED.PinNumber,0);

		case 1: GPIO_WriteToOutputPin(GPIOG,GREEN_LED.PinNumber,0);

	}
}
void turnOnLED(uint8_t led) {
	switch(led) {
		case 0: GPIO_WriteToOutputPin(GPIOG,RED_LED.PinNumber,1);

		case 1: GPIO_WriteToOutputPin(GPIOG,GREEN_LED.PinNumber,1);

	}

}
