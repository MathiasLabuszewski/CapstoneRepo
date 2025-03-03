/*
 * main.c
 *
 *  Created on: Jan 28, 2025
 *      Author: Jacob
 */
#include "applicationCode.h"
#include "scheduler.h"



int main(void) {
	applicationInit();
	uint32_t eventsToRun = getScheduledEvents();
	uint32_t eventsToRunComparisonToggle;
	uint32_t eventsToRunComparisonDelay;
#if !USE_INTERRUPT_FOR_BUTTON
	uint32_t eventsToRunComparisonBTN;
#endif
	for(;;) {
		eventsToRunComparisonToggle = (eventsToRun &=LED_TOGGLE_EVENT);
		eventsToRunComparisonDelay = (eventsToRun &=LED_DELAY_EVENT);
#if !USE_INTERRUPT_FOR_BUTTON
		eventsToRunComparisonBTN = (eventsToRun &=BTN_POLLING_EVENT);
#endif
		if(eventsToRun == (eventsToRunComparisonToggle)) {
			toggleRedLED();
		}
		if(eventsToRun == (eventsToRunComparisonDelay)) {
			appDelay(DELAY);
		}
#if !USE_INTERRUPT_FOR_BUTTON
		if(eventsToRun == (eventsToRunComparisonBTN)) {
			executeBtnRoutine();
		}
#endif
	}
}
