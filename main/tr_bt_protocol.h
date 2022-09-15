/*
 * bt_protocol.h
 *
 *  Created on: Mar 31, 2022
 *      Author: Alessandro Iori
 */

#ifndef CUSTOM_TR_BT_PROTOCOL_H_
#define CUSTOM_TR_BT_PROTOCOL_H_

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------
// Public macros

#define TR_BT_PROTOCOL_STATUS_FALSE        0
#define TR_BT_PROTOCOL_STATUS_TRUE         1

#define TR_BT_PROTOCOL_STATUS_ERROR        0
#define TR_BT_PROTOCOL_STATUS_OK           1

// -----------------------------------------------------------------------------
// Public type definitions

typedef uint8_t tr_bt_protocol_status_t;

// -----------------------------------------------------------------------------
// Public variables

// -----------------------------------------------------------------------------
// Public function declarations

void tr_bt_protocol_init(void);
void tr_bt_procotol_event_connection_opened(void);
void tr_bt_procotol_event_connection_closed(void);

//MSG TX
unsigned char* tr_bt_protocol_msg_tx_raw_get(void);
uint8_t tr_bt_protocol_msg_tx_raw_lng_get(void);

void tr_bt_protocol_msg_tx_create_device_info_rsp(void);
void tr_bt_protocol_msg_tx_create_device_status_rsp(void);
void tr_bt_protocol_msg_tx_create_modality_rsp(void);
void tr_bt_protocol_msg_tx_create_modality_event(void);

//MSG RX
void tr_bt_protocol_msg_rx_conn_id_set(uint8_t conn_id);
uint8_t tr_bt_protocol_msg_rx_conn_id_get(void);

uint8_t* tr_bt_protocol_msg_rx_auth_secret_get(void);
uint8_t tr_bt_protocol_msg_rx_auth_secret_lng_get(void);

void tr_bt_protocol_msg_rx_activity(unsigned char *raw, uint8_t raw_lng, char auth);


#endif /* CUSTOM_TR_BT_PROTOCOL_H_ */
