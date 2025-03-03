/*
 * scheduler.c
 *
 *  Created on: Jan 28, 2025
 *      Author: Jacob
 */

#include "scheduler.h"

uint32_t scheduledEvent;
uint32_t getScheduledEvents() {
	return scheduledEvent;
}
void addSchedulerEvent(uint32_t event) {
	scheduledEvent |= event;
}
void removeScehdulerEvent(uint32_t event) {
	scheduledEvent &= ~event;

}
