/*
 * applicationCode.h
 *
 *  Created on: Jan 28, 2025
 *      Author: Jacob
 */

#ifndef APPLICATIONCODE_H_
#define APPLICATIONCODE_H_



#endif /* APPLICATIONCODE_H_ */
#include "LEDDriver.h"
#include "scheduler.h"
#include "Button_Driver.h"

#define USE_INTERRUPT_FOR_BUTTON 0

#define NAMELENGTH 5
#define DELAY 250000

void applicationInit();
void greenLEDInit();
void redLEDInit();
void toggleGreenLED();
void toggleRedLED();
void actvateGreenLED();
void activateRedLED();
void deactivateGreenLED();
void deactivateRedLED();
void appDelay(uint32_t delay);

void btnInit();
void executeBtnRoutine();
void btnInteruptInit();
