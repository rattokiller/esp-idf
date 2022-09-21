/*
 * controller.c
 *
 *  Created on: Apr 1, 2022
 *      Author: Alessandro Iori
 */


// -----------------------------------------------------------------------------
// Includes

#include <FILO_BLE/common.h>
#include <FILO_BLE/controller.h>
#include <FILO_BLE/sl_simple_timer.h>
#include <FILO_BLE/state_event.h>
#include "esp_log.h"

#define GATTS_TAG "GATTS_iBeacon"
// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------
// Private macros
// -----------------------------------------------------------------------------
// Private type definitions
// -----------------------------------------------------------------------------
// Private variables

static uint8_t _controller_modality_type = CONTROLLER_MODALITY_TYPE_DEFAULT;
static uint8_t _controller_modality_intensity = CONTROLLER_MODALITY_INTENSITY_DEFAULT;
static uint8_t _controller_modality_time = CONTROLLER_MODALITY_TIME_DEFAULT;
static uint8_t _controller_modality_elapsed_time = CONTROLLER_MODALITY_ELAPSED_TIME_DEFAULT;
static uint8_t _controller_modality_status = CONTROLLER_MODALITY_STATUS_DEFAULT;


static uint8_t _controller_modality_rqt_source = CONTROLLER_MODALITY_RQT_SOURCE_DEFAULT; //iunternal or externale
static uint8_t _controller_modality_rqt_status = CONTROLLER_MODALITY_RQT_STATUS_DEFAULT;

static uint8_t _controller_modality_type_rqt = CONTROLLER_MODALITY_TYPE_RQT_DEFAULT;
static uint8_t _controller_modality_intensity_rqt = CONTROLLER_MODALITY_INTENSITY_RQT_DEFAULT;
static uint8_t _controller_modality_time_rqt = CONTROLLER_MODALITY_TIME_RQT_DEFAULT;

uint16_t  DRV10975SpeedTable[] = { 0, 80, 100, 120, 135, 150, 165, 180, 200, 220};
uint16_t  vibratorSpeedTable[] = { 0, 35, 50, 70,  90, 110, 130, 150, 170, 190};

static int _controller_task_sm = 0;
static sl_simple_timer_t _controller_task_timer;

// -----------------------------------------------------------------------------
// Private function declarations
static void _controller_task_timer_cb(sl_simple_timer_t *timer, void *data);
static void _control_modality_task(void);
// -----------------------------------------------------------------------------
// Private function definitions

static void _controller_task_timer_cb(sl_simple_timer_t *timer, void *data)
{
  (void) timer;
  (void) data;
  _controller_task_sm++;
}

static void _control_modality_task(void)
{
 /*
  MP2615task();
  DRV10975Task();
  vibratorTask();

  //CHARGER MANAGER
  //TODO: add interrupt on source presence and removed

  state_e state = tr_se_state_get();
  if (MP2615_status.sourcePresent)
    {

      if(state != CHARGING_STATE)
        {
          tr_se_event_generate(CHARGER_IN_EVENT);
        }

      if (MP2615_status.charging)
        {
          led_rgb_set(COLOR_ORANGE); //TODO: ORANGE
        }
      else
        {
          led_rgb_set(COLOR_GREEN);
        }
    }
  else
    {
      if(state == CHARGING_STATE)
        {
          tr_se_event_generate(CHARGER_OUT_EVENT);
        }
    }
    */
}

// -----------------------------------------------------------------------------
// Public function definitions

void controller_init(void)
{
  //MP2615int();
  //DRV10975Init(0);
  //vibratorInit(0);

  sl_status_t sc;
  sc = sl_simple_timer_start(&_controller_task_timer,
                             100,
                             _controller_task_timer_cb,
                             NULL, true);
  //app_assert_status(sc);
}

void control_process_action(void)
{
  if (_controller_task_sm)
    {
      _controller_task_sm--;
      _control_modality_task();
    }
}

void control_modality_stop(void)
{
  //DRV10975Stop();
  //vibratorStop();
}

uint8_t controller_modality_type_get(void)
{
  return _controller_modality_type;
}

uint8_t controller_modality_intensity_get(void)
{
  return _controller_modality_intensity;
}

uint8_t controller_modality_time_get(void)
{
  return _controller_modality_time;
}

uint8_t controller_modality_elapsed_time_get(void)
{
  //TODO: get from timer
  return _controller_modality_elapsed_time;
}

uint8_t controller_modality_status_get(void)
{
  return _controller_modality_status;
}

void controller_modality_time_set(uint8_t time)
{
  _controller_modality_time = time;

  //TODO: set end timer
}

void controller_modality_intensity_set(uint8_t intensity)
{
  _controller_modality_intensity = intensity;
}

void controller_modality_type_set(uint8_t type)
{
  _controller_modality_type = type;
}

void controller_modality_status_set(uint8_t status)
{
  _controller_modality_status = status;
}

void controller_modality_set(uint8_t type, uint8_t intensity, uint8_t time, uint8_t status)
{
  _controller_modality_type = type;
  _controller_modality_intensity = intensity;
  _controller_modality_time = time;
  _controller_modality_status = status;

  if(_controller_modality_status == CONTROLLER_MODALITY_STATUS_OK)
    {
      //TODO: set end timer
    }

}

//MODALITY RQT (request to validdate)

uint8_t controller_modality_type_rqt_get(void)
{
  return _controller_modality_type_rqt;
}

uint8_t controller_modality_intensity_rqt_get(void)
{
  return _controller_modality_intensity_rqt;
}

uint8_t controller_modality_time_rqt_get(void)
{
  return _controller_modality_time_rqt;
}

uint8_t controller_modality_rqt_source_get(void)
{
  return _controller_modality_rqt_source;
}

uint8_t controller_modality_rqt_status_get(void)
{
  return _controller_modality_rqt_status;
}

controller_status_t controller_modality_rqt_check(uint8_t type_rqt, uint8_t intensity_rqt, uint8_t time_rqt)
{
  if(type_rqt < CONTROLLER_MODALITY_TYPE_CONF_MIN ||
      type_rqt > CONTROLLER_MODALITY_TYPE_CONF_MAX)
    {
      ESP_LOGE(GATTS_TAG,"tr controller: error modality type not allowed\r\n");
      return CONTROLLER_STATUS_ERROR;
    }

  if((intensity_rqt + 1) < (CONTROLLER_MODALITY_INTENSITY_CONF_MIN + 1) || //+1 only for build warning
      intensity_rqt > CONTROLLER_MODALITY_INTENSITY_CONF_MAX)
    {
      ESP_LOGE(GATTS_TAG,"tr controller: error modality intensity not allowed\r\n");
      return CONTROLLER_STATUS_ERROR;
    }

  if(time_rqt != 0 && //TODO: create CONTROLLER_MODALITY_TIME_SEC_CONF_NO_CHANGE     0
      (time_rqt < CONTROLLER_MODALITY_TIME_SEC_CONF_MIN ||
      time_rqt > CONTROLLER_MODALITY_TIME_SEC_CONF_MAX))
    {
      ESP_LOGE(GATTS_TAG,"tr controller: error modality time not allowed\r\n");
      return CONTROLLER_STATUS_ERROR;
    }

  return CONTROLLER_STATUS_OK;
}

void controller_modality_rqt_status_set(uint8_t rqt_status)
{
  _controller_modality_rqt_status = rqt_status;
}

void controller_modality_rqt_set(uint8_t type_rqt, uint8_t intensity_rqt, uint8_t time_rqt, uint8_t rqt_source)
{
  _controller_modality_type_rqt = type_rqt;
  _controller_modality_intensity_rqt = intensity_rqt;
  _controller_modality_time_rqt = time_rqt;
  _controller_modality_rqt_source = rqt_source;
}

void controller_modality_rqt_send(void)
{
  //sl_bt_external_signal(EXTERNAL_SIGNAL_MODALITY_RQT);
}

void controller_modality_rqt_activity(void)
{
//  uint8_t external_signal = EXTERNAL_SIGNAL_NULL;
  uint8_t mod_type_rqt = controller_modality_type_rqt_get();
  uint8_t mod_int_rqt = controller_modality_intensity_rqt_get();
  uint8_t mod_time_rqt = controller_modality_time_rqt_get();
  uint8_t mod_rqt_source = controller_modality_rqt_source_get(); //internal/external


  //TODO put  inside controller function
  ESP_LOGI(GATTS_TAG,"tr modality rqt: type %d, intesity %d, time %d, rqt_source: %d\r\n",
               mod_type_rqt,
               mod_int_rqt,
               mod_time_rqt,
               mod_rqt_source);

  controller_status_t check_result =
      controller_modality_rqt_check(mod_type_rqt,
                                       mod_int_rqt,
                                       mod_time_rqt);

  if(check_result == CONTROLLER_STATUS_ERROR)
    {
      ESP_LOGI(GATTS_TAG,"tr modality rqt: error param\r\n");
      controller_modality_rqt_status_set(CONTROLLER_MODALITY_RQT_STATUS_ERROR_1);
    }
  else
    {
      //TODO: send to micro and check ok
      uint8_t micror_result = 0;

    /*
      if(mod_type_rqt  == CONTROLLER_MODALITY_TYPE_ONDULATION)
        {
          led_rgb_set(COLOR_WHITE);
          vibratorStop();
          sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM0);
          tyDRV10975Status motor_status = DRV10975StartAt(DRV10975SpeedTable[mod_int_rqt]);
          sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);

          if(motor_status.fail == 0)
            {
              micror_result = 1;
            }
          else
            {
              app_log_info("motor fail\r\n");
              DRV10975Init(0);
            }
          //          micror_result = 1;
        }
      else
        {
          led_rgb_set(COLOR_PURPLE);
          DRV10975Stop();
          tyVibratorStatus vibrator_status = vibratorStartAt(vibratorSpeedTable[mod_int_rqt]);
          if(vibrator_status.fail == 0)
            {
              micror_result = 1;
            }
          else
            {
              app_log_info("vibrator fail\r\n");
              vibratorInit(0);
            }
        }

      if(micror_result == 1)
        {
          //SUCCESS
          app_log_info("tr micro rsp: success\r\n"); app_log_nl();

          controller_modality_rqt_status_set(CONTROLLER_MODALITY_RQT_STATUS_OK);
          controller_modality_set(mod_type_rqt,
                                     mod_int_rqt,
                                     mod_time_rqt,
                                     CONTROLLER_MODALITY_STATUS_OK);
        }
      else

        {
          //ERROR
          app_log_info("tr micro rsp: error\r\n"); app_log_nl();
          controller_modality_rqt_status_set(CONTROLLER_MODALITY_RQT_STATUS_ERROR_2);
        }
    */
    }

}
