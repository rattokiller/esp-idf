/*
 * tr_state_event.h
 *
 *  Created on: Jul 19, 2022
 *      Author: Alessandro
 */

#ifndef CUSTOM_TR_STATE_EVENT_H_
#define CUSTOM_TR_STATE_EVENT_H_


#include "common.h"

// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------
// Public macro
// -----------------------------------------------------------------------------
// Public type definitions

typedef enum {
  UNKNOWN_STATE,
  IDLE_STATE,
#if !defined(SHUTDOWN_DISABLED)
  WAKEUP_STATE,
#else
  POWER_OFF_STATE,
#endif
  //ACTIVE_STATE,
  UNASSOCIATED_STATE,
  ASSOCIATED_STATE,
  DISCOVERABLE_STATE,
  CONNECTED_STATE,
  FACTORY_RESET_STATE,
  CHARGING_STATE,
} state_e;

typedef enum {
  UNKNOWN_EVENT,

#if !defined(SHUTDOWN_DISABLED)
  WAKEUP_EVENT,
#else
  BOOT_EVENT,
#endif
  IDLE_EVENT,
  ACTIVE_EVENT,
  DISCOVERABLE_EVENT,
  CONNECTION_EVENT,
  DISCONNECTION_EVENT,
  FACTORY_RESET_ENABLED_EVENT,
  FACTORY_RESET_CONFIRMED_EVENT,
  CHARGER_IN_EVENT,
  CHARGER_OUT_EVENT,
} event_e;

// -----------------------------------------------------------------------------
// Public variables
// -----------------------------------------------------------------------------
// Public function declarations
// -----------------------------------------------------------------------------
// Public function definitions

state_e tr_se_state_get(void);

void tr_se_event_generate(event_e new_event);
void tr_se_event_scheduled_generate(event_e new_event_scheduled, uint8_t seconds);

#if !defined(SHUTDOWN_DISABLED)
state_e tr_se_event_wakeup_handler(void);
#endif
state_e tr_se_event_idle_handler(void);
state_e tr_se_event_active_handler(void);
state_e tr_se_event_discoverable_handler(void);
state_e tr_se_event_connection_handler(void);
state_e tr_se_event_disconnection_handler(void);
state_e tr_se_event_factory_reset_enabled_handler(void);
state_e tr_se_event_factory_reset_confirmed_handler(void);
state_e tr_se_event_charger_in_handler(void);
state_e tr_se_event_charger_out_handler(void);

#endif /* CUSTOM_TR_STATE_EVENT_H_ */
