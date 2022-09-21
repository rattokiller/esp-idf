
#include 	"ble_config.h"
#include 	"signal.h"
#include 	"sdkconfig.h"
#include 	"../FILO_BLE/shared_key.h"
#include 	"../FILO_BLE/controller.h"
#include    "../FILO_BLE/connection.h"
#include    "../FILO_BLE/bt_protocol.h"
#include 	"../FILO_BLE/common.h"

void	_bt_on_event(uint32_t external_signal_in){

    ESP_LOGI(GATTS_TAG,"external signal event\r\n");
    uint32_t external_signal = EXTERNAL_SIGNAL_NULL;
    uint32_t  sc;

    connection_t* conn;

    switch (external_signal_in)
    {
      case EXTERNAL_SIGNAL_SHARED_KEY_ACTIVITY:
        ESP_LOGI(GATTS_TAG,"external signal: shared key activity\r\n");

        conn = connection_get(bt_protocol_msg_rx_conn_id_get());

        shared_key_status_t shared_key_activity_result;
        shared_key_activity_result = shared_key_activity(bt_protocol_msg_rx_auth_secret_get(),
                                                         bt_protocol_msg_rx_auth_secret_lng_get());

        if(shared_key_activity_result == SHARED_KEY_STATUS_TRUE)
          {
            ESP_LOGI(GATTS_TAG,"shared key activity: success\r\n");
            connection_status_update(conn->id,
                                     CONNECTION_STATUS_TYPE_AUTH,
                                     CONNECTION_STATUS_OK);
          } else {
              ESP_LOGI(GATTS_TAG,"shared key activity: failure\r\n");
              //TODO: sostituire
              //sc = sl_bt_connection_close(conn->id);
              //app_assert_status(sc);
          }
        break;

      case EXTERNAL_SIGNAL_BT_PROTOCOL_TX:
    	/*
        if(connection_get_connection_list_lng() > 0)
          {
            ESP_LOGI(GATTS_TAG,"tr notify: bt protocol tx\r\n");
            /*
            sc = sl_bt_gatt_server_notify_all(gattdb_protocol_tx,
                                              sizeof(unsigned char)*tr_bt_protocol_msg_tx_raw_lng_get(),
                                              (uint8_t*)tr_bt_protocol_msg_tx_raw_get());
            //app_assert_status(sc);
             *

          }
        else
          {
            ESP_LOGI(GATTS_TAG,"tr notify: bt protocol tx: error not connected\r\n");
          }

    	 */
    	  tr_bt_protocol_msg_tx_raw_get();

    	  rsp.attr_value.handle = ble_connect.param->read.handle;
    	                  rsp.attr_value.len = 20;
    	                  rsp.attr_value.value[0] = 0x01;
    	                  rsp.attr_value.value[1] = 0x01;

    	                  gatts_TX.attr_value[2]= count;
    	                  rsp.attr_value.value[2] = count;

    	                  rsp.attr_value.value[3] = 0xff;

          esp_ble_gatts_set_attr_value(gl_profile_tab[PROFILE_A_APP_ID].char_handle_TX, //deve essere attribute hendeler
        		  	  	  	  	  	  sizeof(unsigned char)*tr_bt_protocol_msg_tx_raw_lng_get(),
									  bt_protocol_msg_tx_raw_get()
                                      );

          esp_ble_gatts_send_indicate(ble_connect.gatts_if,
                                      ble_connect.param->read.conn_id,
                                      gl_profile_tab[PROFILE_A_APP_ID].char_handle_TX,
									  sizeof(unsigned char)*tr_bt_protocol_msg_tx_raw_lng_get(),
									  bt_protocol_msg_tx_raw_get(),
                                      true);

    	  ESP_LOGI(GATTS_TAG,"tr notify: bt protocol tx\r\n");

        break;


      case EXTERNAL_SIGNAL_MODALITY_RQT:

        ESP_LOGI(GATTS_TAG,"tr modality rqt\r\n");
        //wdog_feed();

        controller_modality_rqt_activity();

        uint8_t mod_rqt_source = controller_modality_rqt_source_get(); //internal/external

        if(mod_rqt_source == CONTROLLER_MODALITY_RQT_SOURCE_INT)
          {
            //TODO: check rqt_status != ERROR
            ESP_LOGI(GATTS_TAG,"tr rqt from type internal\r\n");
            bt_protocol_msg_tx_create_modality_event();
            external_signal = EXTERNAL_SIGNAL_BT_PROTOCOL_TX;
          }
        else if(mod_rqt_source == CONTROLLER_MODALITY_RQT_SOURCE_EXT)
          {
            ESP_LOGI(GATTS_TAG,"tr rqt from type external\r\n");
            bt_protocol_msg_tx_create_modality_rsp();
            external_signal = EXTERNAL_SIGNAL_BT_PROTOCOL_TX;
          }
        else
          {
            ESP_LOGI(GATTS_TAG,"tr rqt from unknown\r\n");
          }
        break;

      default:
        break;
    }

    if(external_signal != EXTERNAL_SIGNAL_NULL)
      {
        //Call external signal
        sl_bt_external_signal(external_signal);
      }

}




uint32_t sl_bt_external_signal(uint32_t signal){

	_bt_on_event(signal);

	return 0;

}


