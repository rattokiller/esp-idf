/*
 * tr_state_event.c
 *
 *  Created on: Jul 19, 2022
 *      Author: Alessandro
 */

// -----------------------------------------------------------------------------
// Includes

#include "sl_simple_timer.h"

#include "common.h"

#include "tr_state_event.h"


#include "esp_log.h"

#define GATTS_TAG "GATTS_TR_STATE"

// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------
// Private macro
// -----------------------------------------------------------------------------
// Private type definitions



//SOURCE: https://www.embedded.com/implementing-finite-state-machines-in-embedded-systems/


// -----------------------------------------------------------------------------
// Private variables

#if !defined(SHUTDOWN_DISABLED)
state_e _state = IDLE_STATE;
#else
state_e _state = POWER_OFF_STATE;
#endif
state_e _state_next = UNKNOWN_STATE;
event_e _event = UNKNOWN_EVENT;
event_e _event_scheduled = UNKNOWN_EVENT;

static sl_simple_timer_t _event_scheduled_timer;


const char * const _state_str[] =
    {
        [UNKNOWN_STATE] = "UNKNOWN_STATE",
        [IDLE_STATE] = "IDLE_STATE",
#if !defined(SHUTDOWN_DISABLED)
        [WAKEUP_STATE] = "WAKEUP_STATE",
#else
        [POWER_OFF_STATE] = "POWER_OFF_STATE",
#endif
        //[ACTIVE_STATE] = "ACTIVE_STATE",
        [UNASSOCIATED_STATE] = "UNASSOCIATED_STATE",
        [ASSOCIATED_STATE] = "ASSOCIATED_STATE",
        [DISCOVERABLE_STATE] = "DISCOVERABLE_STATE",
        [CONNECTED_STATE] = "CONNECTED_STATE",
        [FACTORY_RESET_STATE] = "FACTORY_RESET_STATE",
        [CHARGING_STATE] = "CHARGING_STATE",
    };

const char * const _event_str[] =
    {
      [UNKNOWN_EVENT] = "UNKNOWN_EVENT",
#if !defined(SHUTDOWN_DISABLED)
      [WAKEUP_EVENT] = "WAKEUP_EVENT",
#else
      [BOOT_EVENT] = "BOOT_EVENT",
#endif
      [IDLE_EVENT] = "IDLE_EVENT",
      [ACTIVE_EVENT] = "ACTIVE_EVENT",
      [DISCOVERABLE_EVENT] = "DISCOVERABLE_EVENT",
      [CONNECTION_EVENT] = "CONNECTION_EVENT",
      [DISCONNECTION_EVENT] = "DISCONNECTION_EVENT",
      [FACTORY_RESET_ENABLED_EVENT] = "FACTORY_RESET_ENABLED_EVENT",
      [FACTORY_RESET_CONFIRMED_EVENT] = "FACTORY_RESET_CONFIRMED_EVENT",
      [CHARGER_IN_EVENT] = "CHARGER_IN_EVENT",
      [CHARGER_OUT_EVENT] = "CHARGER_OUT_EVENT",
    };

// -----------------------------------------------------------------------------
// Private function declarations

static void _event_scheduled_cb(sl_simple_timer_t *timer, void *data);

static void _event_handler(void);
//#if !defined(SHUTDOWN_DISABLED)
//static state_e _event_wakeup_handler(void);
//#endif
//static state_e _event_idle_handler(void);
//static state_e _event_active_handler(void);
//static state_e _event_discoverable_handler(void);
//static state_e _event_connection_handler(void);
//static state_e _event_disconnection_handler(void);
//static state_e _event_factory_reset_enabled_handler(void);
//static state_e _event_factory_reset_confirmed_handler(void);
//static state_e _event_charger_in_handler(void);
//static state_e _event_charger_out_handler(void);

// -----------------------------------------------------------------------------
// Private function definitions

static void _event_scheduled_cb(sl_simple_timer_t *timer, void *data)
{
  (void) timer;
  (void) data;
  /*
  _event = _event_next;
  _event_next = UNKNOWN_EVENT;
  _event_handler();
  */

ESP_LOGW(GATTS_TAG,"event scheduled callback: event_scheduled %s, state %s\r\n", _event_str[_event_scheduled], _state_str[_state]); 

  if(
#if !defined(SHUTDOWN_DISABLED)
      (_state == WAKEUP_STATE && _event_scheduled == IDLE_EVENT) ||
#endif
      (_state == DISCOVERABLE_STATE && _event_scheduled == ACTIVE_EVENT) ||
      (_state == FACTORY_RESET_STATE && _event_scheduled == ACTIVE_EVENT)
      )
    {
      ESP_LOGI(GATTS_TAG,"event scheduled callback OK\r\n"); 
      tr_se_event_generate(_event_scheduled);
    }
  else
    {
      ESP_LOGI(GATTS_TAG,"event scheduled callback KO\r\n"); 
    }

  //clean next event
  _event_scheduled = UNKNOWN_EVENT;
}

static void _event_handler(void)
{
  ESP_LOGI(GATTS_TAG,"event handler"); 
  //clean state next
  _state_next = UNKNOWN_STATE;

  //check new event presence
  if(_event == UNKNOWN_EVENT)
    {
      ESP_LOGI(GATTS_TAG,"event handler: error: event unknown"); 
      return;
    }

  switch (_state)
  {

#if !defined(SHUTDOWN_DISABLED)
    case IDLE_STATE:
      if(_event == WAKEUP_EVENT)
        {
          _state_next = tr_se_event_wakeup_handler();
        }
      break;

    case WAKEUP_STATE:
      if(_event == IDLE_EVENT)
        {
          _state_next = tr_se_event_idle_handler();
        }
      else if (_event == ACTIVE_EVENT)
        {
          _state_next = tr_se_event_active_handler(); // UNASSOCIATED_STATE or ASSOCIATED_STATE
        }
      else if(_event == FACTORY_RESET_ENABLED_EVENT)
        {
          _state_next = tr_se_event_factory_reset_enabled_handler();
        }
      break;
#else
    case POWER_OFF_STATE:
      if (_event == BOOT_EVENT)
        {
          _state_next = tr_se_event_idle_handler();
        }
      break;

    case IDLE_STATE:
      if (_event == ACTIVE_EVENT)
        {
          _state_next = tr_se_event_active_handler(); // UNASSOCIATED_STATE or ASSOCIATED_STATE
        }
      else if(_event == FACTORY_RESET_ENABLED_EVENT)
        {
          _state_next = tr_se_event_factory_reset_enabled_handler();
        }
      else if(_event == CHARGER_IN_EVENT)
        {
          _state_next = tr_se_event_charger_in_handler();
        }
      break;
#endif
      /*
    case ACTIVE_STATE:
      break;
      */
    case UNASSOCIATED_STATE:
      if(_event == IDLE_EVENT)
        {
          _state_next = tr_se_event_idle_handler();
        }
      else if (_event == DISCOVERABLE_EVENT)
        {
          _state_next = tr_se_event_discoverable_handler();
        }
      else if(_event == FACTORY_RESET_ENABLED_EVENT)
        {
          _state_next = tr_se_event_factory_reset_enabled_handler();
        }
      else if(_event == CHARGER_IN_EVENT)
        {
          _state_next = tr_se_event_charger_in_handler();
        }
      break;

    case ASSOCIATED_STATE:
      if(_event == IDLE_EVENT)
        {
          _state_next = tr_se_event_idle_handler();
        }
      else if (_event == DISCOVERABLE_EVENT)
        {
          _state_next = tr_se_event_discoverable_handler();
        }
      else if (_event == CONNECTION_EVENT)
        {
          _state_next = tr_se_event_connection_handler();
        }
      else if(_event == FACTORY_RESET_ENABLED_EVENT)
        {
          _state_next = tr_se_event_factory_reset_enabled_handler();
        }
      else if(_event == CHARGER_IN_EVENT)
        {
          _state_next = tr_se_event_charger_in_handler();
        }
      break;

    case DISCOVERABLE_STATE:
      if(_event == IDLE_EVENT)
        {
          _state_next = tr_se_event_idle_handler();
        }
      else if (_event == ACTIVE_EVENT)
        {
          _state_next = tr_se_event_active_handler();
        }
      else if (_event == CONNECTION_EVENT)
        {
          _state_next = tr_se_event_connection_handler();
        }
      else if(_event == FACTORY_RESET_ENABLED_EVENT)
        {
          _state_next = tr_se_event_factory_reset_enabled_handler();
        }
      else if(_event == CHARGER_IN_EVENT)
        {
          _state_next = tr_se_event_charger_in_handler();
        }
      break;

    case CONNECTED_STATE:
      if(_event == IDLE_EVENT)
        {
          _state_next = tr_se_event_idle_handler();
        }
      else if(_event == DISCONNECTION_EVENT)
        {
          _state_next = tr_se_event_disconnection_handler();
        }
      else if(_event == FACTORY_RESET_ENABLED_EVENT)
        {
          _state_next = tr_se_event_factory_reset_enabled_handler();
        }
      else if(_event == CHARGER_IN_EVENT)
        {
          _state_next = tr_se_event_charger_in_handler();
        }
      break;

    case FACTORY_RESET_STATE:
      if (_event == ACTIVE_EVENT)
        {
          _state_next = tr_se_event_active_handler();
        }
      else if(_event == FACTORY_RESET_CONFIRMED_EVENT)
        {
          _state_next = tr_se_event_factory_reset_confirmed_handler();
        }
      else if(_event == CHARGER_IN_EVENT)
        {
          _state_next = tr_se_event_charger_in_handler();
        }
      break;

    case CHARGING_STATE:
      if(_event == CHARGER_OUT_EVENT)
        {
          _state_next = tr_se_event_charger_out_handler();
        }
      break;

    default:
      break;
  }

  if(_state_next != UNKNOWN_STATE)
    {
      //ESP_LOGI(GATTS_TAG,"event handler: old_state %s event %s state %s", _state, _event, _state_next); 
      ESP_LOGI(GATTS_TAG,"event handler: old_state %s, event %s, state %s", _state_str[_state], _event_str[_event], _state_str[_state_next]); 
      _state = _state_next;
    }
  else
    {
      ESP_LOGI(GATTS_TAG,"event handler: error: event %s state %s", _event_str[_event], _state_str[_state]); 
    }

  //clean event
  _event = UNKNOWN_EVENT;
}



// -----------------------------------------------------------------------------
// Public function definitions

state_e tr_se_state_get(void)
{
  return _state;
}

void tr_se_event_generate(event_e new_event)
{
  _event = new_event;
  _event_handler();
}

void tr_se_event_scheduled_generate(event_e new_event_scheduled, uint8_t seconds)
{
  _event_scheduled = new_event_scheduled;

  sl_status_t sc;
  sc = sl_simple_timer_start(&_event_scheduled_timer,
                             seconds * 1000,
                             _event_scheduled_cb,
                             NULL, false);
  //app_assert_status(sc);
}

//Functions defined in main code


//#if !defined(SHUTDOWN_DISABLED)
//state_e _event_wakeup_handler(void)
//{
//  ESP_LOGI(GATTS_TAG,"event wakeup handler"); 
//
//  state_e result_state;
//
//  _event_scheduled = IDLE_EVENT;
//
//  sl_status_t sc;
//  sc = sl_simple_timer_start(&_event_scheduled_timer,
//                             16 * 1000,
//                             _event_scheduled_cb,
//                             NULL, false);
//  app_assert_status(sc);
//
//  result_state = WAKEUP_STATE;
//
//  return result_state;
//}
//#endif
//
//static state_e _event_idle_handler(void)
//{
//  ESP_LOGI(GATTS_TAG,"event idle handler"); 
//  state_e result_state;
//
//  led_rgb_off();
//
//  DRV10975Stop();
//  vibratorStop();
//
//  advertise_stop();
//
//  sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
//
//  shutdown();
//
//  result_state = IDLE_STATE;
//
//  return result_state;
//}
//
//static state_e _event_active_handler(void)
//{
//  ESP_LOGI(GATTS_TAG,"event active handler"); 
//  state_e result_state;
//
//  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
//
//  if(shared_key_presence_get() == SHARED_KEY_PRESENCE_TRUE)
//    {
//      result_state = ASSOCIATED_STATE;
//      advertise_stop();
//      ESP_LOGI(GATTS_TAG,"advertise start\r\n"); 
//      advertise_start(adv_invterval_slow, false);
//    } else {
//        result_state = UNASSOCIATED_STATE;
//
//        ESP_LOGI(GATTS_TAG,"advertise stop\r\n"); 
//        advertise_stop();
//    }
//
//  uint8_t modality_type = tr_controller_modality_type_get();
//  uint8_t modality_intensity = tr_controller_modality_intensity_get();
//  uint8_t modality_time = tr_controller_modality_time_get();
//
//  if(_state == IDLE_STATE ||
//      _state == DISCOVERABLE_STATE)
//    {
//      modality_intensity = 0;
//      modality_time = 0;
//    }
//
//  //Enter modality type
//  tr_controller_modality_rqt_set(modality_type, modality_intensity, modality_time, TR_CONTROLLER_MODALITY_RQT_SOURCE_UNKNOWN);
//  sl_bt_external_signal(EXTERNAL_SIGNAL_MODALITY_RQT);
//
//  return result_state;
//}
//
//static state_e _event_discoverable_handler(void)
//{
//  ESP_LOGI(GATTS_TAG,"event discoverable handler"); 
//  state_e result_state;
//
//  led_rgb_set(COLOR_BLUE);
//
//  _event_scheduled = ACTIVE_EVENT;
//
//  sl_status_t sc;
//  sc = sl_simple_timer_start(&_event_scheduled_timer,
//                             30 * 1000,
//                             _event_scheduled_cb,
//                             NULL, false);
//  app_assert_status(sc);
//
//  DRV10975Stop();
//  vibratorStop();
//
//  advertise_stop();
//  ESP_LOGI(GATTS_TAG,"advertise start\r\n"); 
//  advertise_start(adv_invterval_fast, true);
//
//  result_state  = DISCOVERABLE_STATE;
//
//  return result_state;
//}
//
//static state_e _event_connection_handler(void)
//{
//  ESP_LOGI(GATTS_TAG,"event connection handler"); 
//  state_e result_state;
//
//  if(connection_get_connection_list_lng() < CONNECTION_LIST_MAX_LNG)
//    {
//      advertise_start(adv_invterval_slow, false);
//    } else {
//        ESP_LOGI(GATTS_TAG,"advertise stop.\r\n"); 
//        advertise_stop();
//    }
//
//  tr_bt_procotol_event_connection_opened();
//
//
//  uint8_t modality_type = tr_controller_modality_type_get();
//  uint8_t modality_intensity = tr_controller_modality_intensity_get();
//  uint8_t modality_time = tr_controller_modality_time_get();
//
//  if(_state == DISCOVERABLE_STATE)
//    {
//      modality_intensity = 0;
//      modality_time = 0;
//    }
//
//  //Enter modality type
//  tr_controller_modality_rqt_set(modality_type, modality_intensity, modality_time, TR_CONTROLLER_MODALITY_RQT_SOURCE_UNKNOWN);
//  sl_bt_external_signal(EXTERNAL_SIGNAL_MODALITY_RQT);
//
//  result_state  = CONNECTED_STATE;
//
//  return result_state;
//}
//
//static state_e _event_disconnection_handler(void)
//{
//  ESP_LOGI(GATTS_TAG,"event disconnection handler"); 
//  state_e result_state;
//
//  if(connection_get_connection_list_lng() == 0)
//    {
//      connection_stop_timer(); // Stop timer used for check connections status
//    }
//
//  tr_bt_procotol_event_connection_closed();
//
//  result_state =  _event_active_handler();
//
//  return result_state;
//}
//
//
//static state_e _event_factory_reset_enabled_handler(void)
//{
//  ESP_LOGI(GATTS_TAG,"event reset enabled handler"); 
//  state_e result_state;
//
//  _event_scheduled = ACTIVE_EVENT;
//
//  sl_status_t sc;
//  sc = sl_simple_timer_start(&_event_scheduled_timer,
//                             5 * 1000,
//                             _event_scheduled_cb,
//                             NULL, false);
//  app_assert_status(sc);
//
//  result_state = FACTORY_RESET_STATE;
//
//  return result_state;
//}
//
//static state_e _event_factory_reset_confirmed_handler(void)
//{
//  ESP_LOGI(GATTS_TAG,"event reset confirmed handler"); 
//  state_e result_state;
//
//  shared_key_erease();
//
//  result_state = _event_active_handler();
//
//  return result_state;
//}
//
//
//static state_e _event_charger_in_handler(void)
//{
//  ESP_LOGI(GATTS_TAG,"event charger in handler"); 
//  state_e result_state;
//
//  DRV10975Stop();
//  vibratorStop();
//
//  result_state = CHARGING_STATE;
//
//  return result_state;
//}
//
//static state_e _event_charger_out_handler(void)
//{
//  ESP_LOGI(GATTS_TAG,"event charger out handler"); 
//  state_e result_state;
//
//  DRV10975Stop();
//  vibratorStop();
//
//  result_state = _event_active_handler();
//
//  return result_state;
//}
