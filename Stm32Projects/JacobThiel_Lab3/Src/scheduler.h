/*
 * scheduler.h
 *
 *  Created on: Jan 28, 2025
 *      Author: Jacob
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#define LED_TOGGLE_EVENT (1 << 0)
#define LED_DELAY_EVENT (1 << 1) //we will continue to add events like this for future labs
#define BTN_POLLING_EVENT (1 << 2)


#include <stdint.h>

#endif /* SCHEDULER_H_ */
uint32_t getScheduledEvents();
void addSchedulerEvent(uint32_t event);
void removeScehdulerEvent(uint32_t event);
