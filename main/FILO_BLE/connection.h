/*
 * connection.h
 *
 *  Created on: Nov 22, 2021
 *      Author: Alessandro Iori
 */

#ifndef CUSTOM_CONNECTION_H_
#define CUSTOM_CONNECTION_H_


// -----------------------------------------------------------------------------
// Includes

#include "../FILO_BLE/common.h"

// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------
// Public macros

#define CONNECTION_LIST_MAX_LNG   SL_BT_CONFIG_MAX_CONNECTIONS

#define CONNECTION_STATUS_ERROR   0
#define CONNECTION_STATUS_OK      1

#define CONNECTION_STATUS_TYPE_CONNECTION     1
#define CONNECTION_STATUS_TYPE_AUTH           2
#define CONNECTION_STATUS_TYPE_NOTIFY         3

// -----------------------------------------------------------------------------
// Public type definitions

typedef uint8_t connection_status_t;

typedef uint8_t connection_status_type_t;

typedef struct connection connection_t;

struct connection {
  uint8_t id;
  connection_status_t connection;
  connection_status_t auth;
  connection_status_t notify;
  connection_t *next; /* ptr to next node */
};

// -----------------------------------------------------------------------------
// Public variables
// -----------------------------------------------------------------------------
// Public function declarations
// -----------------------------------------------------------------------------

void connection_init(void);
connection_t* connection_get_connection_list(void);
uint8_t connection_get_connection_list_lng(void);
connection_status_t connection_add(uint8_t connection_id);
connection_status_t connection_remove(uint8_t connection_id);
connection_t* connection_get(uint8_t connection_id);
connection_status_t connection_status_update(uint8_t connection_id, connection_status_type_t status_type, connection_status_t new_status);
//connection_status_t connection_print(uint8_t connection_id);

// Periodic timer callback. Implemented on main code.


//TODO void connection_timer_cb(sl_simple_timer_t *timer, void *data);
// Periodic start timer
void connection_stop_timer(void);
// Periodic stop timer
void connection_start_timer(void);


#endif /* CUSTOM_CONNECTION_H_ */
