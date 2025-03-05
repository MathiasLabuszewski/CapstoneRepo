/*
 * utils.c
 *
 *  Created on: Jan 21, 2025
 *  Author: Mathias Labuszewski
 */


/*
 * UTILS.c
 * This file contains all the utilities for the stm32wle we are using.
 * This will have things like the UUID address and other stuff with mehtods of getting those things out
 *
 * For board (pcb) spefiic things, the BSP file will have the defines (gpio ports/pins...)
 *
 *
 */

#include "utils.h"


uint64_t UID_GetFullUID(void) {
    uint32_t uidl = *((uint32_t *)(UID64_BASE_ADDRESS));
    uint32_t uidh = *((uint32_t *)(UID64_BASE_ADDRESS + UID_OFFSET_STID));
    return (((uint64_t)uidh) << 32) | uidl;
}


uint32_t UID_GetDeviceNumber(void) {
    return *((uint32_t *)(UID64_BASE_ADDRESS + UID_OFFSET_DEVNUM));
}


uint32_t UID_GetCompanyID(void) {
    uint32_t stid = *((uint32_t *)(UID64_BASE_ADDRESS + UID_OFFSET_STID));
    return (stid >> 8) & 0xFFFFFF;
}


uint8_t UID_GetDeviceID(void) {
    uint32_t stid = *((uint32_t *)(UID64_BASE_ADDRESS + UID_OFFSET_STID));
    return (uint8_t)(stid & 0xFF);
}


uint8_t UID_GetPackageType(void) {
    uint32_t pkg_reg = *((uint32_t *)(UID64_BASE_ADDRESS));
    return (uint8_t)(pkg_reg & 0x1F);
}




