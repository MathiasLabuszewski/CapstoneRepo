/*
 * GPIODriver.h
 *
 *  Created on: Jan 28, 2025
 *      Author: Jacob
 */

#ifndef GPIODRIVER_H_
#define GPIODRIVER_H_

#define GPIO_PIN_0 0
#define GPIO_PIN_1 1
#define GPIO_PIN_2 2
#define GPIO_PIN_3 3
#define GPIO_PIN_4 4
#define GPIO_PIN_5 5
#define GPIO_PIN_6 6
#define GPIO_PIN_7 7
#define GPIO_PIN_8 8
#define GPIO_PIN_9 9
#define GPIO_PIN_10 10
#define GPIO_PIN_11 11
#define GPIO_PIN_12 12
#define GPIO_PIN_13 13
#define GPIO_PIN_14 14
#define GPIO_PIN_15 15

#define GPIO_MODE_INPUT 0b00
#define GPIO_MODE_OUTPUT 0b01
#define GPIO_MODE_AFM 0b10
#define GPIO_MODE_ANA 0b11

#define GPIO_OPTYPE_PUSHPULL 0
#define GPIO_OPTYPE_OPENDRAIN 1

#define GPIO_SPEED_LOW 0b00
#define GPIO_SPEED_MEDIUM 0b01
#define GPIO_SPEED_HIGH 0b10
#define GPIO_SPEED_VERY 0b11

#define GPIO_PUPD_NONE 0b00
#define GPIO_PUPD_UP 0b01
#define GPIO_PUPD_DOWN 0b10
#define GPIO_PUPD_RES 0b11

#define ledshift 2

#define NO_INTERUPT 0b00
#define FALLING_INTERUPT 0b01
#define RISING_INTERUPT 0b10
#define FALLING_RISING_INTERUPT 0b11

#define GPIOA_PORT 0
#define GPIOB_PORT 1
#define GPIOC_PORT 2
#define GPIOD_PORT 3
#define GPIOE_PORT 4
#define GPIOF_PORT 5
#define GPIOG_PORT 6
#define GPIOH_PORT 7
#define GPIOI_PORT 8
#define GPIOJ_PORT 9
#define GPIOK_PORT 10

#define fourDivisor 4
#define fullByte 0xF
#include "InteruptControl.h"

typedef struct {
	uint8_t PinNumber;
	uint8_t PinMode;
	uint8_t OPType;
	uint8_t PinSpeed;
	uint8_t PinPuPdControl;
	uint8_t PinIntMode;

}GPIO_PinConfig_t;

void GPIO_Init(GPIO_RegDef_t * initRegDefPointer, GPIO_PinConfig_t);
void GPIO_ClockControl(GPIO_RegDef_t * controlRegDefPointer, uint8_t toggle);
void GPIO_WriteToOutputPin(GPIO_RegDef_t * writeRegDefPointer, uint8_t pinNumber, uint8_t value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t * toggleRegDefPointer, uint8_t pinNumber);

uint8_t GPIO_ReadOutputPin(GPIO_RegDef_t * readOutputPointer, uint8_t pinNumber);
uint16_t GPIO_ReadPort(GPIO_RegDef_t * readPort, uint16_t pinNumber);

void GPIO_WriteGPIOInterupt(uint8_t IRQ_Number, uint8_t toggle);

#endif /* GPIODRIVER_H_ */

