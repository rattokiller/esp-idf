/*
 * common.h
 *
 *  Created on: Oct 27, 2021
 *      Author: Alessandro Iori
 */

#ifndef CUSTOM_COMMON_H_
#define CUSTOM_COMMON_H_
// -----------------------------------------------------------------------------
// Includes

#include "string.h"

// -----------------------------------------------------------------------------
// Configuration

#define SHUTDOWN_DISABLED

// -----------------------------------------------------------------------------
// Private macros

#define TICK_PER_SECOND        (uint32_t)(32768) // sl_sleeptimer_get_timer_frequency();
#define HALF_TICK_PER_SECOND   (uint32_t)(TICK_PER_SECOND/2)

//EXTERNAL SIGNAL
#define EXTERNAL_SIGNAL_NULL                       0
#define EXTERNAL_SIGNAL_SHARED_KEY_ACTIVITY        1
#define EXTERNAL_SIGNAL_BT_PROTOCOL_TX             2
#define EXTERNAL_SIGNAL_MODALITY_RQT               3
// -----------------------------------------------------------------------------
// Private type definitions
// -----------------------------------------------------------------------------
// Private variables
// -----------------------------------------------------------------------------
// Private function declarations
// -----------------------------------------------------------------------------
// Private function definitions
// -----------------------------------------------------------------------------
// Public function definitions


#endif /* CUSTOM_COMMON_H_ */
