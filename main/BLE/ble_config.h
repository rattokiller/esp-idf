#ifndef  __BLE_CONFIG_H__
#define __BLE_CONFIG_H__


#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"

#define GATTS_TAG "GATTS_iBeacon"

#define gattdb_generic_attribute              1
#define gattdb_service_changed_char           3
#define gattdb_database_hash                  6
#define gattdb_client_support_features        8
#define gattdb_device_name                    11
#define gattdb_device_information             14
#define gattdb_manufacturer_name_string       16
#define gattdb_system_id                      18
#define gattdb_serial_number_string           20
#define gattdb_hardware_revision_string       22
#define gattdb_firmware_revision_string       24
#define gattdb_protocol_tx                    27
#define gattdb_protocol_rx                    30


///Declare the static function
static void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

//A è piu completo, B dovrebbe esserre l' essenziale che ci serve

//#define GATTS_SERVICE_UUID_TEST_A   

static uint8_t GATTS_SERVICE_UUID_TEST_A[32] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    //first uuid, 16bit, [12],[13] is the value
       0x78,  0x10,  0xdc,  0x2f,0x35,0x69, 0x70,0xb5, 0x67, 0x4d,  0x9e,0xad, 0xeb,0x48, 0xea, 0xc9,
};
static uint8_t GATTS_CHAR_UUID_TEST_TX[16] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    //first uuid, 16bit, [12],[13] is the value
    0x39, 0x0B, 0xA5, 0xA2, 0x87, 0x61, 0xAC, 0xAC, 0xFB, 0x4C, 0x2B, 0xA6, 0xE9, 0x42, 0xEB, 0x54,
    //0xc9, 0xea, 0x48, 0xeb, 0xad, 0x9e, 0x4d, 0x67, 0xb5, 0x70, 0x69, 0x35, 0x2f, 0xdc, 0x10, 0x78,
    
};
static uint8_t GATTS_CHAR_UUID_TEST_RX[16] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    //first uuid, 16bit, [12],[13] is the value
           
    0x83,0x73,0x52,0x7F,0x04,0x5E, 0x70,0xAB,0x0E,0x4F,0x45, 0xC3, 0x64, 0x3B,0x58, 0x43,
    
};
//#define GATTS_CHAR_UUID_TEST_TX     0x54EB42E9A62B4CFBACAC6187A2A50B39
//#define GATTS_CHAR_UUID_TEST_RX     0x43583B64C3454F0EAB705E047F527383

#define GATTS_NUM_HANDLE_TEST_A     6 //2



#define TEST_DEVICE_NAME            "iBeacon"
#define TEST_MANUFACTURER_DATA_LEN  20

#define GATTS_DEMO_CHAR_VAL_LEN_MAX  0x20 // 0x14 //20

#define PREPARE_BUF_MAX_SIZE 1024

static uint8_t char_TX[20];
static uint8_t char_RX[20];
static esp_gatt_char_prop_t a_property = 0;


static esp_attr_value_t gatts_TX =
{
    .attr_max_len = GATTS_DEMO_CHAR_VAL_LEN_MAX,
    .attr_len     = sizeof(char_TX),
    .attr_value   = char_TX,
};

static esp_attr_value_t gatts_RX =
{
    .attr_max_len = GATTS_DEMO_CHAR_VAL_LEN_MAX,
    .attr_len     = sizeof(char_RX),
    .attr_value   = char_RX,
};

static uint8_t adv_config_done = 0;
#define adv_config_flag      (1 << 0)
#define scan_rsp_config_flag (1 << 1)

#ifdef CONFIG_SET_RAW_ADV_DATA
static uint8_t raw_adv_data[] = {
        0x02, 0x01, 0x06,
        0x02, 0x0a, 0xeb, 0x03, 0x03, 0xab, 0xcd
};
static uint8_t raw_scan_rsp_data[] = {
        0x0f, 0x09, 0x45, 0x53, 0x50, 0x5f, 0x47, 0x41, 0x54, 0x54, 0x53, 0x5f, 0x44,
        0x45, 0x4d, 0x4f
};
#else

static uint8_t adv_service_uuid128[32] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    //first uuid, 16bit, [12],[13] is the value
    
    0x53, 0x41, 0x33, 0x30, 0x39, 0x31, 0x4b, 0x32, 0x41, 0x54, 0x41, 0x54, 0x4f, 0x4c, 0x49, 0x46,
    0x46, 0x49, 0x4c, 0x4f, 0x54, 0x41, 0x54, 0x41, 0x32, 0x4b, 0x31, 0x39, 0x30, 0x33, 0x41, 0x53,
    
    //second uuid, 32bit, [12], [13], [14], [15] is the value
   
};

// The length of adv data must be less than 31 bytes
//static uint8_t test_manufacturer[TEST_MANUFACTURER_DATA_LEN] =  {0x12, 0x23, 0x45, 0x56};
//adv data
static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = true,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x0006, //slave connection min interval, Time = min_interval * 1.25 msec
    .max_interval = 0x0020, //slave connection max interval, Time = max_interval * 1.25 msec
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data =  NULL, //&test_manufacturer[0],
    .service_data_len = 0,//2?
    .p_service_data = NULL,
    .service_uuid_len = sizeof(adv_service_uuid128),
    .p_service_uuid = adv_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};
// scan response data
static esp_ble_adv_data_t scan_rsp_data = {
    .set_scan_rsp = false,
    .include_name = false,
    .include_txpower = false,
    //.min_interval = 0x0006,
    //.max_interval = 0x0010,
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data =  NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(adv_service_uuid128),
    .p_service_uuid = adv_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

#endif /* CONFIG_SET_RAW_ADV_DATA */

static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x40,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    //.peer_addr            =
    //.peer_addr_type       =
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

#define PROFILE_NUM 1 //1
#define PROFILE_A_APP_ID 0


struct gatts_profile_inst {
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_handle_TX;
    uint16_t char_handle_RX;
    esp_bt_uuid_t char_uuid_TX;
    esp_bt_uuid_t char_uuid_RX;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
};

/* One gatt-based profile one app_id and one gatts_if, this array will store the gatts_if returned by ESP_GATTS_REG_EVT */
static struct gatts_profile_inst gl_profile_tab[PROFILE_NUM] = {

    [PROFILE_A_APP_ID] = {
        .gatts_cb = gatts_profile_a_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
     
};

typedef struct {
    uint8_t                 *prepare_buf;
    int                     prepare_len;
} prepare_type_env_t;

static prepare_type_env_t a_prepare_write_env;


#endif
