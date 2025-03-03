/*
 * applicationCode.c
 *
 *  Created on: Jan 28, 2025
 *      Author: Jacob
 */


#include "applicationCode.h"

void greenLEDInit() {
	LED_Init(GREEN);
}
void redLEDInit() {
	LED_Init(RED);
}
void toggleGreenLED() {
	toggleLED(GREEN);
}
void toggleRedLED() {
	toggleLED(RED);
}
void activateGreenLED() {
	turnOnLED(GREEN);
}
void activateRedLED() {
	turnOnLED(RED);
}
void deactivateGreenLED() {
	turnOffLED(GREEN);
}
void deactivateRedLED() {
	turnOffLED(RED);
}
void btnInit() {
	BTN_Init();
}
void btnInteruptInit() {
	BTN_Interupt_Init();
}
void executeBtnRoutine() {
	if(BTN_Pressed()) {
		activateGreenLED();
	}
	else {
		deactivateGreenLED();
	}
}
void appDelay(uint32_t delay) {
	char name[NAMELENGTH] = "Jacob";
		[[maybe_unused]] char destinationArray[NAMELENGTH];
		for(uint32_t i = 0; i<delay;i++) {
			for(uint32_t j = 0; j<NAMELENGTH;j++) {
				destinationArray[j] = name[j];
			}
		}
}
void applicationInit() {
	//redLEDInit();
	greenLEDInit();
	deactivateGreenLED();

#if !USE_INTERRUPT_FOR_BUTTON
	btnInit();
	addSchedulerEvent(BTN_POLLING_EVENT);
#elif USE_INTERRUPT_FOR_BUTTON == ACTIVE
	btnInteruptInit();
#else
#error "USE_INTERUPT_FOR_BUTTON must be 0 or 1"
#endif

	//addSchedulerEvent(LED_TOGGLE_EVENT);
	addSchedulerEvent(LED_DELAY_EVENT);
}
	void EXTI0_IRQHandler(void) {
		IRQ_Disable(EXTI0_IRQ_NUMBER);
		toggleGreenLED();
		EXTI_Clear(GPIO_PIN_0);
		IRQ_Enable(EXTI0_IRQ_NUMBER);
	}
