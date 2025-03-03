/*
 * InteruptControl.h
 *
 *  Created on: Feb 20, 2025
 *      Author: Jacob
 */

#ifndef INTERUPTCONTROL_H_
#define INTERUPTCONTROL_H_


#endif /* INTERUPTCONTROL_H_ */
#include "STM32F429i.h"
#include <stdint.h>
#include <stdbool.h>
#define EXTI0_IRQ_NUMBER 6

void IRQ_Enable(uint8_t interuptNumber);
void IRQ_Disable(uint8_t interuptNumber);
void IRQ_Clear(uint8_t interuptNumber);
void IRQ_Set(uint8_t interuptNumber);

void EXTI_Clear(uint8_t interuptNumber);
