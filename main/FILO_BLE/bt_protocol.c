/*
 * bt_protocol.c
 *
 *  Created on: Mar 31, 2022
 *      Author: Alessandro Iori
 */

// -----------------------------------------------------------------------------
// Includes
#include "../FILO_BLE/bt_protocol.h"


#include <stdbool.h>


#include "../FILO_BLE/common.h"

#include "../FILO_BLE/controller.h"

#include "../BLE/signal.h"
#include "esp_log.h"

#define GATTS_TAG "GATTS_iBeacon"

// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------
// Private macros

/*
 * MESSAGES
 */

#define BT_PROTOCOL_MSG_RX_CONN_ID_DEFAULT               (uint8_t) 0
#define BT_PROTOCOL_MSG_RX_SECRET_LNG                    (uint8_t) 32
#define BT_PROTOCOL_MSG_RX_SECRET_DEFAULT                123


#define BT_PROTOCOL_MSG_SIZE_MAX                          (uint8_t) 20
#define BT_PROTOCOL_MSG_SIZE_MIN                          (uint8_t) 2

#define BT_PROTOCOL_MSG_FIELD_MSGCODE_LNG                 (uint8_t) 1
#define BT_PROTOCOL_MSG_FIELD_MSGCODE_BYTE_POSTION        (uint8_t) 1
#define BT_PROTOCOL_MSG_FIELD_MSGCODE_MIN                 0x01
#define BT_PROTOCOL_MSG_FIELD_MSGCODE_MAX                 0x0b
#define BT_PROTOCOL_MSG_FIELD_MSGCODE_UNAUTH_MIN          0x01
#define BT_PROTOCOL_MSG_FIELD_MSGCODE_UNAUTH_MAX          0x03

#define BT_PROTOCOL_MSG_FIELD_SEQUENCE_LNG                (uint8_t) 1
#define BT_PROTOCOL_MSG_FIELD_SEQUENCE_BYTE_POSTION       (uint8_t) 0
#define BT_PROTOCOL_MSG_FIELD_SEQUENCE_MAX                0xff
#define BT_PROTOCOL_MSG_FIELD_SEQUENCE_MIN                0x00
#define BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT            BT_PROTOCOL_MSG_FIELD_SEQUENCE_MIN

#define BT_PROTOCOL_MSG_FIELD_FWV_LNG                     10 //DEVICE_INFO_FWV_LEN
#define BT_PROTOCOL_MSG_FIELD_FWV_MAJ_DEFAULT             0x00
#define BT_PROTOCOL_MSG_FIELD_FWV_MIN_DEFAULT             0x00
#define BT_PROTOCOL_MSG_FIELD_FWV_REL_DEFAULT             0x00
#define BT_PROTOCOL_MSG_FIELD_HWV_LNG                     DEVICE_INFO_FWV_LEN
#define BT_PROTOCOL_MSG_FIELD_HWV_MAJ_DEFAULT             0x00
#define BT_PROTOCOL_MSG_FIELD_HWV_MIN_DEFAULT             0x00
#define BT_PROTOCOL_MSG_FIELD_HWV_REL_DEFAULT             0x00
#define BT_PROTOCOL_MSG_FIELD_MAC_LNG                     DEVICE_INFO_MAC_LEN
#define BT_PROTOCOL_MSG_FIELD_MAC_DEFAULT                 {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}
#define BT_PROTOCOL_MSG_FIELD_SERIAL_LNG                  4
#define BT_PROTOCOL_MSG_FIELD_SERIAL_DEFAULT              {0xff, 0xff, 0xff, 0xff}

//TODO use default value from tr_controllor.h lib
#define BT_PROTOCOL_MSG_FIELD_BAT_PTC_DEFAULT             0x00
#define BT_PROTOCOL_MSG_FIELD_MOD_TYPE_DEFAULT            0x00
#define BT_PROTOCOL_MSG_FIELD_MOD_INTENSITY_DEFAULT       0x00
#define BT_PROTOCOL_MSG_FIELD_MOD_TIME_DEFAULT            0x00
#define BT_PROTOCOL_MSG_FIELD_MOD_ELAPSED_TIME_DEFAULT    0x00
#define BT_PROTOCOL_MSG_FIELD_MOD_STATUS_DEFAULT          0x00

//MSG

#define BT_PROTOCOL_MSG_DEVICE_INFO_RQT_FIELD_MSGCODE        0x01
#define BT_PROTOCOL_MSG_DEVICE_INFO_RQT_SIZE                 (uint8_t) 2 //2+0

#define BT_PROTOCOL_MSG_DEVICE_INFO_RQT_DEFAULT                            \
  {                                                                           \
    .sequence           = BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT,          \
    .msgcode            = BT_PROTOCOL_MSG_DEVICE_INFO_RQT_FIELD_MSGCODE    \
  }


#define BT_PROTOCOL_MSG_DEVICE_INFO_RSP_FIELD_MSGCODE        0x02
#define BT_PROTOCOL_MSG_DEVICE_INFO_RSP_SIZE                 (uint8_t) 19 //2+17

#define BT_PROTOCOL_MSG_DEVICE_INFO_RSP_DEFAULT                            \
		{                                                                         \
	.sequence           = BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT,          \
	.msgcode            = BT_PROTOCOL_MSG_DEVICE_INFO_RSP_FIELD_MSGCODE,   \
	.hwv_maj            = BT_PROTOCOL_MSG_FIELD_HWV_MAJ_DEFAULT,               \
	.hwv_min            = BT_PROTOCOL_MSG_FIELD_HWV_MIN_DEFAULT,               \
	.hwv_rel            = BT_PROTOCOL_MSG_FIELD_HWV_REL_DEFAULT,               \
	.fwv_maj            = BT_PROTOCOL_MSG_FIELD_FWV_MAJ_DEFAULT,               \
	.fwv_min            = BT_PROTOCOL_MSG_FIELD_FWV_MIN_DEFAULT,               \
	.fwv_rel            = BT_PROTOCOL_MSG_FIELD_FWV_REL_DEFAULT,               \
	.sn                 = BT_PROTOCOL_MSG_FIELD_SERIAL_DEFAULT,            \
	.mac                = BT_PROTOCOL_MSG_FIELD_MAC_DEFAULT,               \
	.bat_ptc            = BT_PROTOCOL_MSG_FIELD_BAT_PTC_DEFAULT            \
		}


#define BT_PROTOCOL_MSG_AUTH_RQT_FIELD_MSGCODE               0x03
#define BT_PROTOCOL_MSG_AUTH_RQT_SIZE                        (uint8_t) 6 //2+4

#define BT_PROTOCOL_MSG_AUT_RQT_DEFAULT                                    \
  {                                                                           \
    .sequence           = BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT,          \
    .msgcode            = BT_PROTOCOL_MSG_AUTH_RQT_FIELD_MSGCODE,          \
    .secret             = SHARED_KEY_DEFAULT_VALUE,                           \
  }


#define BT_PROTOCOL_MSG_DEVICE_STATUS_RQT_FIELD_MSGCODE      0x04
#define BT_PROTOCOL_MSG_DEVICE_STATUS_RQT_SIZE               (uint8_t) 2 //2+0

#define BT_PROTOCOL_MSG_DEVICE_STATUS_RQT_DEFAULT                          \
  {                                                                        \
    .sequence           = BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT,          \
    .msgcode            = BT_PROTOCOL_MSG_DEVICE_STATUS_RQT_FIELD_MSGCODE  \
  }


#define BT_PROTOCOL_MSG_DEVICE_STATUS_RSP_FIELD_MSGCODE      0x05
#define BT_PROTOCOL_MSG_DEVICE_STATUS_RSP_SIZE               (uint8_t) 8 //2+6

#define BT_PROTOCOL_MSG_DEVICE_STATUS_RSP_DEFAULT                          \
  {                                                                        \
    .sequence           = BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT,          \
    .msgcode            = BT_PROTOCOL_MSG_DEVICE_STATUS_RSP_FIELD_MSGCODE,  \
    .mod_type           = BT_PROTOCOL_MSG_FIELD_MOD_TYPE_DEFAULT,            \
    .mod_intensity      = BT_PROTOCOL_MSG_FIELD_MOD_INTENSITY_DEFAULT,     \
    .mod_time           = BT_PROTOCOL_MSG_FIELD_MOD_TIME_DEFAULT,          \
    .mod_elapsed_time   = BT_PROTOCOL_MSG_FIELD_MOD_ELAPSED_TIME_DEFAULT,  \
    .mod_status         = BT_PROTOCOL_MSG_FIELD_MOD_STATUS_DEFAULT,        \
    .bat_ptc            = BT_PROTOCOL_MSG_FIELD_BAT_PTC_DEFAULT            \
  }


//TODO:
#define BT_PROTOCOL_MSG_DEVICE_CONF_RQT_FIELD_MSGCODE         0x06
#define BT_PROTOCOL_MSG_DEVICE_CONF_RQT_SIZE                  (uint8_t) 2 //2+0

#define BT_PROTOCOL_MSG_DEVICE_CONF_RQT_DEFAULT                            \
  {                                                                        \
    .sequence           = BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT,          \
    .msgcode            = BT_PROTOCOL_MSG_DEVICE_CONF_RQT_FIELD_MSGCODE    \
  }

//TODO:
#define BT_PROTOCOL_MSG_DEVICE_CONF_RSP_FIELD_MSGCODE         0x07
#define BT_PROTOCOL_MSG_DEVICE_CONF_RSP_SIZE                  (uint8_t) 2 //2+0

#define BT_PROTOCOL_MSG_DEVICE_CONF_RSP_DEFAULT                            \
  {                                                                        \
    .sequence           = BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT,          \
    .msgcode            = BT_PROTOCOL_MSG_DEVICE_CONF_RSP_FIELD_MSGCODE    \
  }

#define BT_PROTOCOL_MSG_MODALITY_RQT_FIELD_MSGCODE                 0x08
#define BT_PROTOCOL_MSG_MODALITY_RQT_SIZE                          (uint8_t) 5 //2+3

#define BT_PROTOCOL_MSG_MODALITY_RQT_DEFAULT                                 \
  {                                                                          \
    .sequence           = BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT,            \
    .msgcode            = BT_PROTOCOL_MSG_DEVICE_MODALITY_RQT_FIELD_MSGCODE  \
    .mod_type           = BT_PROTOCOL_MSG_FIELD_MOD_TYPE_DEFAULT,              \
    .mod_intensity      = BT_PROTOCOL_MSG_FIELD_MOD_INTENSITY_DEFAULT,       \
    .mod_time           = BT_PROTOCOL_MSG_FIELD_MOD_TIME_DEFAULT,            \
  }

#define BT_PROTOCOL_MSG_DEVICE_MODALITY_RSP_FIELD_MSGCODE           0x09
#define BT_PROTOCOL_MSG_DEVICE_MODALITY_RSP_SIZE                    (uint8_t) 3 //2+1

#define BT_PROTOCOL_MSG_MODALITY_RSP_DEFAULT                                 \
  {                                                                          \
    .sequence           = BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT,            \
    .msgcode            = BT_PROTOCOL_MSG_DEVICE_MODALITY_RSP_FIELD_MSGCODE,  \
    .mod_rqt_status     = BT_PROTOCOL_MSG_FIELD_MOD_STATUS_DEFAULT,          \
  }

#define BT_PROTOCOL_MSG_DEVICE_MODALITY_EVENT_FIELD_MSGCODE          0x0a
#define BT_PROTOCOL_MSG_DEVICE_MODALITY_EVENT_SIZE                   (uint8_t) 6 //2+4

#define BT_PROTOCOL_MSG_MODALITY_EVENT_DEFAULT                                 \
  {                                                                          \
    .sequence           = BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT,            \
    .msgcode            = BT_PROTOCOL_MSG_DEVICE_MODALITY_EVENT_FIELD_MSGCODE,  \
    .mod_type           = BT_PROTOCOL_MSG_FIELD_MOD_TYPE_DEFAULT,              \
    .mod_intensity      = BT_PROTOCOL_MSG_FIELD_MOD_INTENSITY_DEFAULT,       \
    .mod_time           = BT_PROTOCOL_MSG_FIELD_MOD_TIME_DEFAULT,            \
    .mod_status         = BT_PROTOCOL_MSG_FIELD_MOD_STATUS_DEFAULT,          \
  }

// -----------------------------------------------------------------------------
// Private type definitions


// BT_PROTOCOL_MSG_DEVICE_INFO_RQT
typedef union
{
  unsigned char   raw[BT_PROTOCOL_MSG_DEVICE_INFO_RQT_SIZE];
  struct
  {
    uint8_t sequence;              // Sequence field.
    uint8_t msgcode;               // Opcode field.
  };
} bt_protocol_msg_device_info_rqt_t;


// BT_PROTOCOL_MSG_DEVICE_INFO_RSP
typedef union
{
  unsigned char   raw[BT_PROTOCOL_MSG_DEVICE_INFO_RSP_SIZE];
  struct
  {
    uint8_t sequence;              // Sequence field.
    uint8_t msgcode;               // Opcode field.
    uint8_t hwv_maj;
    uint8_t hwv_min;
    uint8_t hwv_rel;
    uint8_t fwv_maj;
    uint8_t fwv_min;
    uint8_t fwv_rel;
    uint8_t sn[6];//BT_PROTOCOL_MSG_FIELD_MAC_DEFAULT
    uint8_t mac[4];//[DEVICE_INFO_MAC_LEN];
    uint8_t bat_ptc;
  };
} bt_protocol_msg_device_info_rsp_t;


//BT_PROTOCOL_MSG_AUTH_RQT
typedef union
{
  unsigned char   raw[BT_PROTOCOL_MSG_AUTH_RQT_SIZE];
  struct
  {
    uint8_t sequence;              // Sequence field.
    uint8_t msgcode;               // Opcode field.
    uint8_t secret[BT_PROTOCOL_MSG_RX_SECRET_LNG];
  };
} bt_protocol_msg_auth_rqt_t;


// BT_PROTOCOL_MSG_DEVICE_STATUS_RQT
typedef union
{
  unsigned char   raw[BT_PROTOCOL_MSG_DEVICE_STATUS_RQT_SIZE];
  struct
  {
    uint8_t sequence;              // Sequence field.
    uint8_t msgcode;               // Opcode field.
  };
} bt_protocol_msg_device_status_rqt_t;


//BT_PROTOCOL_MSG_DEVICE_STATUS_RSP
typedef union
{
  unsigned char   raw[BT_PROTOCOL_MSG_DEVICE_STATUS_RSP_SIZE];
  struct
  {
    uint8_t sequence;              // Sequence field.
    uint8_t msgcode;               // Opcode field.
    uint8_t mod_type;                // Modality Type
    uint8_t mod_intensity;         // Modality Intensity
    uint8_t mod_time;              // Modality Time (duration)
    uint8_t mod_elapsed_time;      // Modality Elapsed time
    uint8_t mod_status;            // Modality Status: 0x01 success, 0x02 error 0x03 error ..
    uint8_t bat_ptc;
  };
} bt_protocol_msg_device_status_rsp_t;

//BT_PROTOCOL_MSG_DEVICE_CONF_RQT
typedef union
{
  unsigned char   raw[BT_PROTOCOL_MSG_DEVICE_CONF_RQT_SIZE];
  struct
  {
    uint8_t sequence;              // Sequence field.
    uint8_t msgcode;               // Opcode field.
  };
} bt_protocol_msg_device_conf_rqt_t;


// BT_PROTOCOL_MSG_DEVICE_CONF_RSP
typedef union
{
  unsigned char   raw[BT_PROTOCOL_MSG_DEVICE_CONF_RSP_SIZE];
  struct
  {
    uint8_t sequence;              // Sequence field.
    uint8_t msgcode;               // Opcode field.
  };
} bt_protocol_msg_device_conf_rsp_t;


// BT_PROTOCOL_MSG_DEVICE_MODALITY_RQT
typedef union
{
  unsigned char   raw[BT_PROTOCOL_MSG_MODALITY_RQT_SIZE];
  struct
  {
    uint8_t sequence;              // Sequence field.
    uint8_t msgcode;               // Opcode field.
    uint8_t mod_type;              // Modality type
    uint8_t mod_intensity;         // Modality Intensity
    uint8_t mod_time;              // Modality Time (duration)
  };
} bt_protocol_msg_modality_rqt_t;

// BT_PROTOCOL_MSG_DEVICE_MODALITY_RSP
typedef union
{
  unsigned char   raw[BT_PROTOCOL_MSG_DEVICE_MODALITY_RSP_SIZE];
  struct
  {
    uint8_t sequence;              // Sequence field.
    uint8_t msgcode;               // Opcode field.
    uint8_t mod_rqt_status;        // Modality Requested Status: 0x01 success, 0x02 error 0x03 error ..
  };
} bt_protocol_msg_modality_rsp_t;

// BT_PROTOCOL_MSG_DEVICE_MODALITY_EVENT
typedef union
{
  unsigned char   raw[BT_PROTOCOL_MSG_DEVICE_MODALITY_EVENT_SIZE];
  struct
  {
    uint8_t sequence;              // Sequence field.
    uint8_t msgcode;               // Opcode field.
    uint8_t mod_type;              // Modality Type
    uint8_t mod_intensity;         // Modality Intensity
    uint8_t mod_time;              // Modality Time (duration)
    uint8_t mod_status;            // Modality Status: 0x01 success, 0x02 error 0x03 error ..
  };
} bt_protocol_msg_modality_event_t;

// -----------------------------------------------------------------------------
// Private variables

// MESSAGES

// MSG TX
static uint8_t _bt_protocol_msg_tx_sequence = BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT;
static uint8_t _bt_protocol_msg_rx_sequence = BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT;

static unsigned char *_bt_protocol_msg_tx_raw;
static uint8_t _bt_protocol_msg_tx_raw_lng = 0;

// MSG RX
static uint8_t _bt_protocol_msg_rx_conn_id = BT_PROTOCOL_MSG_RX_CONN_ID_DEFAULT;
static uint8_t _bt_protocol_msg_rx_auth_secret[BT_PROTOCOL_MSG_RX_SECRET_LNG] ; //BT_PROTOCOL_MSG_RX_SECRET_DEFAULT;
static uint8_t _bt_protocol_msg_rx_auth_secret_lng = BT_PROTOCOL_MSG_RX_SECRET_LNG;

// -----------------------------------------------------------------------------
// Private function declarations

// UTILS
void _big_to_little_and_viceversa(uint8_t* raw, uint8_t raw_lng);

// MESSAGE TX
uint8_t _bt_protocol_msg_tx_sequence_get(void);
void _bt_protocol_msg_tx_sequence_increase(void);
void _bt_protocol_msg_tx_raw_create(unsigned char *msg_raw, uint8_t msg_raw_len);

// MESSAGE RX
uint8_t _bt_protocol_msg_rx_sequence_get(void);
void _bt_protocol_msg_rx_sequence_set(uint8_t new_sequence);
bt_protocol_status_t _bt_protocol_msg_rx_format_rules_check(unsigned char *raw, uint8_t raw_lng, char auth);
bt_protocol_status_t _bt_protocol_msg_rx_activity(unsigned char *raw, uint8_t raw_lng, uint8_t* external_signal);

// -----------------------------------------------------------------------------
// Private function definitions

void _big_to_little_and_viceversa(uint8_t* raw, uint8_t raw_lng)
{
  uint8_t raw_copy[raw_lng];
  memcpy(raw_copy, raw, raw_lng);

  for(int i=0; i<raw_lng; i++)
  {
    raw[i] = raw_copy[raw_lng-i-1];
  }
}


// MESSAGE TX
uint8_t _bt_protocol_msg_tx_sequence_get(void)
{
  return _bt_protocol_msg_tx_sequence;
}

void _bt_protocol_msg_tx_sequence_increase(void)
{
  _bt_protocol_msg_tx_sequence = (_bt_protocol_msg_tx_sequence + 1) % BT_PROTOCOL_MSG_FIELD_SEQUENCE_MAX;
}

void _bt_protocol_msg_tx_raw_create(unsigned char *msg_raw, uint8_t msg_raw_len)
{
  //copy
  unsigned char msg_raw_copy[msg_raw_len];
  memcpy(msg_raw_copy, msg_raw, msg_raw_len);

  //clear
  _bt_protocol_msg_tx_raw_lng = 0;
  free(_bt_protocol_msg_tx_raw);

  //create
  _bt_protocol_msg_tx_raw = (unsigned char *) malloc(sizeof(unsigned char) * msg_raw_len);
  _bt_protocol_msg_tx_raw_lng = msg_raw_len;
  memcpy(_bt_protocol_msg_tx_raw, msg_raw_copy, msg_raw_len);

  ESP_LOGI(GATTS_TAG,"bt_protocol msg tx raw created - len: %d\r\n",msg_raw_len);
}


// MESSAGE RX

uint8_t _bt_protocol_msg_rx_sequence_get(void)
{
  return _bt_protocol_msg_rx_sequence;
}

void _bt_protocol_msg_rx_sequence_set(uint8_t new_sequence)
{
  _bt_protocol_msg_rx_sequence = new_sequence;
}

bt_protocol_status_t _bt_protocol_msg_rx_format_rules_check(unsigned char *raw, uint8_t raw_lng, char auth)
{

  if(raw_lng < BT_PROTOCOL_MSG_SIZE_MIN ||
      raw_lng > BT_PROTOCOL_MSG_SIZE_MAX)
    {
      ESP_LOGI(GATTS_TAG,"bt_protocol msg rx check: error size\r\n");
      return BT_PROTOCOL_STATUS_ERROR;
    }

  uint8_t sequence = raw[BT_PROTOCOL_MSG_FIELD_SEQUENCE_BYTE_POSTION];

  if(sequence == _bt_protocol_msg_rx_sequence)
    {
      ESP_LOGI(GATTS_TAG,"bt_protocol msg rx check: error: sequence\r\n");
      return BT_PROTOCOL_STATUS_ERROR;
    }

  uint8_t msgcode = raw[BT_PROTOCOL_MSG_FIELD_MSGCODE_BYTE_POSTION];

  if(msgcode < BT_PROTOCOL_MSG_FIELD_MSGCODE_MIN ||
      msgcode > BT_PROTOCOL_MSG_FIELD_MSGCODE_MAX)
    {
      ESP_LOGI(GATTS_TAG,"bt_protocol msg rx check: error: msgcode unknown\r\n");
      return BT_PROTOCOL_STATUS_ERROR;
    }

  if((auth == false) && (msgcode < BT_PROTOCOL_MSG_FIELD_MSGCODE_UNAUTH_MIN ||
      msgcode > BT_PROTOCOL_MSG_FIELD_MSGCODE_UNAUTH_MAX))
    {
      ESP_LOGI(GATTS_TAG,"bt_protocol msg rx check: error: mscode with auth required\r\n");
      return BT_PROTOCOL_STATUS_ERROR;
    }

  ESP_LOGI(GATTS_TAG,"bt_protocol msg rx check: ok\r\n");
  return BT_PROTOCOL_STATUS_OK;
}

bt_protocol_status_t _bt_protocol_msg_rx_activity(unsigned char *raw, uint8_t raw_lng, uint8_t* external_signal)
{
  (void) external_signal;
  bt_protocol_status_t result = BT_PROTOCOL_STATUS_OK;
  unsigned char raw_copy[raw_lng];
  memcpy(raw_copy, raw, raw_lng);

  uint8_t msgcode = raw_copy[BT_PROTOCOL_MSG_FIELD_MSGCODE_BYTE_POSTION];

  switch (msgcode)
  {
    bt_protocol_msg_modality_rqt_t bt_protocol_msg_modality_rqt;
    bt_protocol_msg_auth_rqt_t bt_protocol_msg_auth_rqt;
    bt_protocol_msg_device_info_rqt_t bt_protocol_msg_device_info_rqt;
    bt_protocol_msg_device_status_rqt_t bt_protocol_msg_device_status_rqt;

    case BT_PROTOCOL_MSG_DEVICE_INFO_RQT_FIELD_MSGCODE:
      ESP_LOGI(GATTS_TAG,"bt_protocol msg rx: BT_PROTOCOL_MSG_DEVICE_INFO_RQT_FIELD_MSGCODE\r\n");

      memcpy(&bt_protocol_msg_device_info_rqt, raw_copy, sizeof(bt_protocol_msg_device_info_rqt_t));
      //check
      if(raw_lng != BT_PROTOCOL_MSG_DEVICE_INFO_RQT_SIZE)
        {
          ESP_LOGI(GATTS_TAG,"bt_protocol msg rx: error msg size\r\n");
          result = BT_PROTOCOL_STATUS_ERROR;
          break;
        }

      //activity
      bt_protocol_msg_tx_create_device_info_rsp();
      *external_signal = EXTERNAL_SIGNAL_BT_PROTOCOL_TX;
      break;

    case BT_PROTOCOL_MSG_AUTH_RQT_FIELD_MSGCODE:
      ESP_LOGI(GATTS_TAG,"bt_protocol msg rx: BT_PROTOCOL_MSG_AUTH_RQT_FIELD_MSGCODE\r\n");
      // 0103DDCCBBAA
      memcpy(&bt_protocol_msg_auth_rqt, raw_copy, sizeof(bt_protocol_msg_auth_rqt_t));

      //check
      if(raw_lng != BT_PROTOCOL_MSG_AUTH_RQT_SIZE)
        {
          ESP_LOGI(GATTS_TAG,"bt_protocol msg rx: error msg size\r\n");
          result = BT_PROTOCOL_STATUS_ERROR;
          break;
        }

      //action
      memcpy(_bt_protocol_msg_rx_auth_secret, bt_protocol_msg_auth_rqt.secret, BT_PROTOCOL_MSG_RX_SECRET_LNG);
      _big_to_little_and_viceversa(_bt_protocol_msg_rx_auth_secret, BT_PROTOCOL_MSG_RX_SECRET_LNG);
      _bt_protocol_msg_rx_auth_secret_lng = BT_PROTOCOL_MSG_RX_SECRET_LNG;

      *external_signal = EXTERNAL_SIGNAL_SHARED_KEY_ACTIVITY;
      break;

    case BT_PROTOCOL_MSG_DEVICE_STATUS_RQT_FIELD_MSGCODE:
      ESP_LOGI(GATTS_TAG,"bt_protocol msg rx: BT_PROTOCOL_MSG_DEVICE_STATUS_RQT_FIELD_MSGCODE\r\n");
      //0204
      memcpy(&bt_protocol_msg_device_status_rqt, raw_copy, sizeof(bt_protocol_msg_device_status_rqt_t));
      //check
      if(raw_lng != BT_PROTOCOL_MSG_DEVICE_STATUS_RQT_SIZE)
        {
          ESP_LOGI(GATTS_TAG,"bt_protocol msg rx: error msg size\r\n");
          result = BT_PROTOCOL_STATUS_ERROR;
          break;
        }

      //action
      bt_protocol_msg_tx_create_device_status_rsp();
      *external_signal = EXTERNAL_SIGNAL_BT_PROTOCOL_TX;
      break;

    case BT_PROTOCOL_MSG_DEVICE_CONF_RQT_FIELD_MSGCODE:
      ESP_LOGI(GATTS_TAG,"bt_protocol msg rx: BT_PROTOCOL_MSG_DEVICE_CONF_RQT_FIELD_MSGCODE\r\n");
      //check
      //action
      break;

    case BT_PROTOCOL_MSG_MODALITY_RQT_FIELD_MSGCODE:
      ESP_LOGI(GATTS_TAG,"bt_protocol msg rx: BT_PROTOCOL_MSG_DEVICE_MODALITY_RQT_FIELD_MSGCODE\r\n");
      //check
      // 0208010101
      memcpy(&bt_protocol_msg_modality_rqt, raw_copy, sizeof(bt_protocol_msg_modality_rqt_t));

      //check
      if(raw_lng != BT_PROTOCOL_MSG_MODALITY_RQT_SIZE)
        {
          ESP_LOGI(GATTS_TAG,"bt_protocol msg rx: error msg size\r\n");
          result = BT_PROTOCOL_STATUS_ERROR;
          break;
        }

      //action
     /*
      tr_controller_status_t check_result =
          tr_controller_modality_rqt_check(bt_protocol_msg_modality_rqt.mod_id,
                                           bt_protocol_msg_modality_rqt.mod_intensity,
                                           bt_protocol_msg_modality_rqt.mod_time);

      if(check_result == TR_CONTROLLER_STATUS_ERROR)
        {
          // create response with error
          tr_controller_modality_rqt_status_set(TR_CONTROLLER_MODALITY_RQT_STATUS_ERROR_1);
          bt_protocol_msg_tx_create_modality_rsp();
          *external_signal = EXTERNAL_SIGNAL_BT_PROTOCOL_TX;

        } else {
        */

      //TODO: sostituire
       
            controller_modality_rqt_set(bt_protocol_msg_modality_rqt.mod_type,
                                           bt_protocol_msg_modality_rqt.mod_intensity,
                                           bt_protocol_msg_modality_rqt.mod_time,
                                           CONTROLLER_MODALITY_RQT_SOURCE_EXT);

            controller_modality_rqt_send();

      
            //*external_signal = EXTERNAL_SIGNAL_MODALITY_RQT;// already done inside rqt

  //}

      break;

    default:
      ESP_LOGI(GATTS_TAG,"bt_protocol msg rx: msgcode unknown\r\n");
      result = BT_PROTOCOL_STATUS_ERROR;
      break;
  }

  return result;
}


// -----------------------------------------------------------------------------
// Public function definitions

void bt_protocol_init(void)
{

}

void bt_procotol_event_connection_opened(void)
{
  //clean conn_id
  _bt_protocol_msg_rx_conn_id = BT_PROTOCOL_MSG_RX_CONN_ID_DEFAULT;

  //clean msg rx auth secret
  _bt_protocol_msg_rx_auth_secret_lng = BT_PROTOCOL_MSG_RX_SECRET_LNG;
  uint8_t auth_secret_default[BT_PROTOCOL_MSG_RX_SECRET_LNG] ;//BT_PROTOCOL_MSG_RX_SECRET_DEFAULT;
  //memcpy(_bt_protocol_msg_rx_auth_secret, auth_secret_default, BT_PROTOCOL_MSG_RX_SECRET_LNG);

  //reset rx/tx sequence
  _bt_protocol_msg_tx_sequence = BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT;
  _bt_protocol_msg_rx_sequence = BT_PROTOCOL_MSG_FIELD_SEQUENCE_DEFAULT;
}

void bt_procotol_event_connection_closed(void)
{

}

//MSG TX

unsigned char* bt_protocol_msg_tx_raw_get(void)
{
  return _bt_protocol_msg_tx_raw;
}

uint8_t bt_protocol_msg_tx_raw_lng_get(void)
{
  return _bt_protocol_msg_tx_raw_lng;
}

void bt_protocol_msg_tx_create_device_info_rsp(void)
{
  ESP_LOGI(GATTS_TAG,"bt_protocol msg tx create: device info rsp\r\n");
  _bt_protocol_msg_tx_sequence_increase();

  bt_protocol_msg_device_info_rsp_t msg_data = BT_PROTOCOL_MSG_DEVICE_INFO_RSP_DEFAULT;

  msg_data.sequence = _bt_protocol_msg_tx_sequence_get();

  uint8_t fwv[BT_PROTOCOL_MSG_FIELD_FWV_LNG];
  //memcpy(fwv, device_info_fwv_get(), BT_PROTOCOL_MSG_FIELD_FWV_LNG);
 /*
  msg_data.fwv_maj = fwv[DEVICE_INFO_FWV_MAJOR_BYTE_POSITION];
  msg_data.fwv_min = fwv[DEVICE_INFO_FWV_MINOR_BYTE_POSITION];
  msg_data.fwv_rel = fwv[DEVICE_INFO_FWV_RELEASE_BYTE_POSITION];

  uint8_t hwv[BT_PROTOCOL_MSG_FIELD_HWV_LNG];
  //memcpy(hwv, device_info_hwv_get(), BT_PROTOCOL_MSG_FIELD_HWV_LNG);
  msg_data.hwv_maj = hwv[DEVICE_INFO_HWV_MAJOR_BYTE_POSITION];
  msg_data.hwv_min = fwv[DEVICE_INFO_HWV_MINOR_BYTE_POSITION];
  msg_data.hwv_rel = fwv[DEVICE_INFO_HWV_RELEASE_BYTE_POSITION];

*/
  //memcpy(msg_data.sn, device_info_serial_get(), BT_PROTOCOL_MSG_FIELD_SERIAL_LNG);
  _big_to_little_and_viceversa(msg_data.sn, BT_PROTOCOL_MSG_FIELD_SERIAL_LNG);

  //memcpy(msg_data.mac, device_info_mac_get(), BT_PROTOCOL_MSG_FIELD_MAC_LNG);
  //_big_to_little_and_viceversa(msg_data.mac, BT_PROTOCOL_MSG_FIELD_MAC_LNG); //its already little endian

  msg_data.bat_ptc = 100;
  ESP_LOGI(GATTS_TAG,"bt_protocol msg 123\r\n");
  _bt_protocol_msg_tx_raw_create(msg_data.raw, sizeof(msg_data.raw)/sizeof(unsigned char));
}

void bt_protocol_msg_tx_create_device_status_rsp(void)
{
  ESP_LOGI(GATTS_TAG,"bt_protocol msg tx create: device status rsp\r\n");
  _bt_protocol_msg_tx_sequence_increase();

  bt_protocol_msg_device_status_rsp_t msg_data = BT_PROTOCOL_MSG_DEVICE_STATUS_RSP_DEFAULT;

  msg_data.sequence = _bt_protocol_msg_tx_sequence_get();

/*
  msg_data.mod_type = tr_controller_modality_type_get();
  msg_data.mod_intensity = tr_controller_modality_intensity_get();
  msg_data.mod_time = tr_controller_modality_time_get();
  msg_data.mod_elapsed_time = tr_controller_modality_elapsed_time_get();
  msg_data.mod_status = tr_controller_modality_status_get();
  */
  msg_data.bat_ptc = 100;

  _bt_protocol_msg_tx_raw_create(msg_data.raw, sizeof(msg_data.raw)/sizeof(unsigned char));
}


void bt_protocol_msg_tx_create_modality_rsp(void)
{
  ESP_LOGI(GATTS_TAG,"bt_protocol msg tx create: modality rsp\r\n");
  _bt_protocol_msg_tx_sequence_increase();

  bt_protocol_msg_modality_rsp_t msg_data = BT_PROTOCOL_MSG_MODALITY_RSP_DEFAULT;

  msg_data.sequence = _bt_protocol_msg_tx_sequence_get();

  msg_data.mod_rqt_status = 0;//tr_controller_modality_rqt_status_get();

  _bt_protocol_msg_tx_raw_create(msg_data.raw, sizeof(msg_data.raw)/sizeof(unsigned char));
}

void bt_protocol_msg_tx_create_modality_event(void)
{
  ESP_LOGI(GATTS_TAG,"bt_protocol msg tx create: modality event\r\n");
  _bt_protocol_msg_tx_sequence_increase();

  bt_protocol_msg_modality_event_t msg_data = BT_PROTOCOL_MSG_MODALITY_EVENT_DEFAULT;

  msg_data.sequence = _bt_protocol_msg_tx_sequence_get();

/*
  msg_data.mod_type = tr_controller_modality_type_get();
  msg_data.mod_intensity = tr_controller_modality_intensity_get();
  msg_data.mod_time = tr_controller_modality_time_get();
  msg_data.mod_status = tr_controller_modality_status_get();

*/
  _bt_protocol_msg_tx_raw_create(msg_data.raw, sizeof(msg_data.raw)/sizeof(unsigned char));
}

//MSG RX

void bt_protocol_msg_rx_conn_id_set(uint8_t conn_id)
{
  _bt_protocol_msg_rx_conn_id = conn_id;
}

uint8_t bt_protocol_msg_rx_conn_id_get(void)
{
  return _bt_protocol_msg_rx_conn_id;
}

uint8_t* bt_protocol_msg_rx_auth_secret_get(void)
{
  return _bt_protocol_msg_rx_auth_secret;
}

uint8_t bt_protocol_msg_rx_auth_secret_lng_get(void)
{
  return _bt_protocol_msg_rx_auth_secret_lng;
}

void bt_protocol_msg_rx_activity(unsigned char *raw, uint8_t raw_lng, char auth)
{
  //COMMON PART
  ESP_LOGI(GATTS_TAG,"bt_protocol msg rx activity %s\r\n", auth ? "auth" : "unauth");

  //CHECK MSG FORMAT
  bt_protocol_status_t check_result = _bt_protocol_msg_rx_format_rules_check(raw, raw_lng, auth);
  if(check_result == BT_PROTOCOL_STATUS_ERROR)
    {
      return;
    }

  bt_protocol_status_t result = BT_PROTOCOL_STATUS_OK;
  uint8_t external_signal = EXTERNAL_SIGNAL_NULL;
  unsigned char raw_copy[raw_lng];
  memcpy(raw_copy, raw, raw_lng);

  uint8_t msg_rx_seq = raw_copy[BT_PROTOCOL_MSG_FIELD_SEQUENCE_BYTE_POSTION];

  result = _bt_protocol_msg_rx_activity(raw_copy, raw_lng, &external_signal);
  ESP_LOGI(GATTS_TAG,"bt_protocol msg rx: %d - post _bt_protocol_msg_rx_activity\r\n",result);
  if(result != BT_PROTOCOL_STATUS_ERROR)
    {
      ESP_LOGI(GATTS_TAG,"bt_protocol msg rx: set new rx sequence\r\n");
      _bt_protocol_msg_rx_sequence_set(msg_rx_seq);
    } else {
        ESP_LOGI(GATTS_TAG,"bt_protocol msg rx: error\r\n");
        return;
    }

  if(external_signal != EXTERNAL_SIGNAL_NULL)
    {
	  //TODO: trovare alternativa !!!
      sl_bt_external_signal(external_signal);
    }
}
