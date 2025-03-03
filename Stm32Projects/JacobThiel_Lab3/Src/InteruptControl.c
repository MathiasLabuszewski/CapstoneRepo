/*
 * InteruptControl.c
 *
 *  Created on: Feb 20, 2025
 *      Author: Jacob
 *      void GPIO_ToggleOutputPin(GPIO_RegDef_t * toggleRegDefPointer, uint8_t pinNumber) {
	if(((toggleRegDefPointer->GPIOG_ODR >> pinNumber) & 0x1) == 0x1) {
		toggleRegDefPointer->GPIOG_ODR &= ~(ACTIVE << pinNumber);
	}
	else {
		toggleRegDefPointer->GPIOG_ODR |= (ACTIVE << pinNumber);
	}

}
 */
#include "InteruptControl.h"

void IRQ_Enable(uint8_t interuptNumber) { //NVIC_ISER0
	if(interuptNumber < 32) {
		*NVIC_ISER0 |= (1 << interuptNumber);
	}
}
void IRQ_Disable(uint8_t interuptNumber) {
	if(interuptNumber < 32) {
		*NVIC_ICER0 |= (ACTIVE << interuptNumber);
	}
}
void IRQ_Clear(uint8_t interuptNumber) {
	if(interuptNumber < 32) {
		*NVIC_ICPR0 |= (ACTIVE << interuptNumber);
	}
}
void IRQ_Set(uint8_t interuptNumber) {
	if(interuptNumber < 32) {
		*NVIC_ISPR0 |= (ACTIVE << interuptNumber);
	}
}

void EXTI_Clear(uint8_t interuptNumber) { //represeting pin number
	EXTI->EXTI_PR |= (ACTIVE << interuptNumber);
}
