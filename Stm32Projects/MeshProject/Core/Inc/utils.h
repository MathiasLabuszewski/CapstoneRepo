/*
 * utils.h
 *
 *  Created on: Jan 21, 2025
 *  Author: Mathias Labuszewski
 */

/*
 * Header file to utils.c
 * This contins defined for the stm32wle. Any board spefic defines are found in the BSP folder
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_

#include <stdint.h>

// Base address for UID register
#define UID64_BASE_ADDRESS     ((uint32_t)0x1FFF7580U)

// Offsets for the UID components
#define UID_OFFSET_DEVNUM      (0x00U)
#define UID_OFFSET_STID        (0x04U)

// Function prototypes
/**
 * @brief Get 64-bit UID as specified by IEEE ID from the UID.
 * @return uint64_t unique device ID.
 */
uint64_t UID_GetFullUID(void);

/**
 * @brief Get device number ID from the UID.
 * @return uint32_t device number.
 */
uint32_t UID_GetDeviceNumber(void);

/**
 * @brief Get company ID from the UID.
 * @return uint32_t company ID.
 */
uint32_t UID_GetCompanyID(void);

/**
 * @brief Get device ID from the UID.
 * @return uint8_t-bit device ID.
 */
uint8_t UID_GetDeviceID(void);

/**
 * @brief Get package type from the UID.
 * @return uint8_t package type, bits 6,7 are null.
 */
uint8_t UID_GetPackageType(void);








#endif /* INC_UTILS_H_ */
