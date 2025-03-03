/*
 * GPIODriver.c
 *
 *  Created on: Jan 28, 2025
 *      Author: Jacob
 */
#include "GPIODriver.h"
void GPIO_Init(GPIO_RegDef_t * initRegDefPointer, GPIO_PinConfig_t initPinConfig) {
	GPIO_ClockControl(initRegDefPointer,ACTIVE);
	SYSCFG_CLOCK_ENABLE;

	uint8_t regSelect = (initPinConfig.PinNumber/fourDivisor);
	uint8_t bitFieldSelect = (initPinConfig.PinNumber%fourDivisor);
	uint16_t port = GPIO_ReadPort(initRegDefPointer, initPinConfig.PinNumber);

	SYSCFG->SYSCFG_EXTICR[regSelect] &= ~(fullByte << bitFieldSelect); //clearing
	SYSCFG->SYSCFG_EXTICR[regSelect] |= (port << bitFieldSelect); //setting !!!!!!!! CHECK IF THIS WILL AUTOMATICALLY CONVERT TO BINARY!

	EXTI->EXTI_IMR &= ~(ACTIVE << initPinConfig.PinNumber);
	EXTI->EXTI_IMR |= (ACTIVE << initPinConfig.PinNumber);

	uint32_t temp = (initPinConfig.PinMode << (ledshift * initPinConfig.PinNumber));

	initRegDefPointer->GPIOG_MODER &= ~(0x3) << (ledshift * initPinConfig.PinNumber);
	initRegDefPointer->GPIOG_MODER |= temp;

	if(initPinConfig.PinIntMode == NO_INTERUPT) { //none
		EXTI->EXTI_RTSR &= ~(ACTIVE << initPinConfig.PinNumber);
		EXTI->EXTI_FTSR &= ~(ACTIVE << initPinConfig.PinNumber);
	}
	else if(initPinConfig.PinIntMode == FALLING_INTERUPT) { //falling
		EXTI->EXTI_FTSR |= (ACTIVE << initPinConfig.PinNumber);
		EXTI->EXTI_RTSR &= ~(ACTIVE << initPinConfig.PinNumber);
	}
	else if(initPinConfig.PinIntMode == RISING_INTERUPT) {
		EXTI->EXTI_RTSR |= (ACTIVE << initPinConfig.PinNumber);
		EXTI->EXTI_FTSR &= ~(ACTIVE << initPinConfig.PinNumber);
	}
	else if(initPinConfig.PinIntMode == FALLING_RISING_INTERUPT) {
		EXTI->EXTI_RTSR |= (ACTIVE << initPinConfig.PinNumber);
		EXTI->EXTI_FTSR |= (ACTIVE << initPinConfig.PinNumber);
	}



	temp = (initPinConfig.PinSpeed << (ledshift * initPinConfig.PinNumber));
	initRegDefPointer->GPIOG_OSPEEDR &= ~(0x3) << (ledshift * initPinConfig.PinNumber);
	initRegDefPointer->GPIOG_OSPEEDR |= temp;

	temp = (initPinConfig.PinPuPdControl << (ledshift * initPinConfig.PinNumber));
	initRegDefPointer->GPIOG_PUPDR &= ~((0x3) << (ledshift * initPinConfig.PinNumber));
	initRegDefPointer->GPIOG_PUPDR |= temp;

	temp = (initPinConfig.OPType << (ledshift * initPinConfig.PinNumber));
	initRegDefPointer->GPIOG_OTYPER &= ~((0x1) << (initPinConfig.PinNumber));
	initRegDefPointer->GPIOG_OTYPER |= temp;
}
void GPIO_ClockControl(GPIO_RegDef_t * controlRegDefPointer, uint8_t toggle) {
	uint8_t offset = 0;

	if(controlRegDefPointer == GPIOA) {
		offset = RCC_AHB1_GPIOA_OFFSET;
	}
	else if(controlRegDefPointer == GPIOB) {
		offset = RCC_AHB1_GPIOB_OFFSET;
	}
	else if(controlRegDefPointer == GPIOC) {
			offset = RCC_AHB1_GPIOC_OFFSET;
		}
	else if(controlRegDefPointer == GPIOD) {
			offset = RCC_AHB1_GPIOD_OFFSET;
		}
	else if(controlRegDefPointer == GPIOE) {
			offset = RCC_AHB1_GPIOE_OFFSET;
		}
	else if(controlRegDefPointer == GPIOF) {
			offset = RCC_AHB1_GPIOF_OFFSET;
		}
	else if(controlRegDefPointer == GPIOG) {
			offset = RCC_AHB1_GPIOG_OFFSET;
		}
	else if(controlRegDefPointer == GPIOH) {
			offset = RCC_AHB1_GPIOH_OFFSET;
		}
	else if(controlRegDefPointer == GPIOI) {
			offset = RCC_AHB1_GPIOI_OFFSET;
		}



	if(toggle == ACTIVE) {
		GPIO_CLOCK_ENABLE(offset);




	}
	else {
		GPIO_CLOCK_DISABLE(offset);


	}

}
void GPIO_WriteToOutputPin(GPIO_RegDef_t * writeRegDefPointer, uint8_t pinNumber, uint8_t value) {
	if(value == ACTIVE) {
		writeRegDefPointer->GPIOG_ODR |= (ACTIVE << pinNumber);

	}
	else {
		writeRegDefPointer->GPIOG_ODR &= ~(ACTIVE << pinNumber);

	}

}
void GPIO_ToggleOutputPin(GPIO_RegDef_t * toggleRegDefPointer, uint8_t pinNumber) {
	if(((toggleRegDefPointer->GPIOG_ODR >> pinNumber) & 0x1) == 0x1) {
		toggleRegDefPointer->GPIOG_ODR &= ~(ACTIVE << pinNumber);
	}
	else {
		toggleRegDefPointer->GPIOG_ODR |= (ACTIVE << pinNumber);
	}

}
uint8_t GPIO_ReadOutputPin(GPIO_RegDef_t * readOutputPointer, uint8_t pinNumber) {
	uint8_t val = ((readOutputPointer->GPIOG_IDR >> pinNumber) & 0x01);
	return val;
}
uint16_t GPIO_ReadPort(GPIO_RegDef_t * readPort, uint16_t pinNumber) {
	if(readPort == GPIOA) {
			return GPIOA_PORT;
		}
		else if(readPort == GPIOB) {
			return GPIOB_PORT;
		}
		else if(readPort == GPIOC) {
			return GPIOC_PORT;
			}
		else if(readPort == GPIOD) {
			return GPIOD_PORT;
			}
		else if(readPort == GPIOE) {
			return GPIOE_PORT;
			}
		else if(readPort == GPIOF) {
			return GPIOF_PORT;
			}
		else if(readPort == GPIOG) {
			return GPIOG_PORT;
			}
		else if(readPort == GPIOH) {
			return GPIOH_PORT;
			}
		else if(readPort == GPIOI) {
			return GPIOI_PORT;
			}
		else {
			return 0;
		}
}
void GPIO_WriteGPIOInterupt(uint8_t IRQ_Number, uint8_t toggle) {
	if(toggle == ACTIVE) {
		IRQ_Enable(IRQ_Number);
	}
	else {
		IRQ_Disable(IRQ_Number);
	}
}

