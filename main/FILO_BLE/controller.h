/*
 * controller.h
 *
 *  Created on: Apr 1, 2022
 *      Author: Alessandro Iori
 */

#ifndef CUSTOM_CONTROLLER_H_
#define CUSTOM_CONTROLLER_H_

#include <stdlib.h>

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------
// Public macros

#define CONTROLLER_STATUS_FALSE        0
#define CONTROLLER_STATUS_TRUE         1

#define CONTROLLER_STATUS_ERROR        0
#define CONTROLLER_STATUS_OK           1

/*
 * MODALITY
 */

// Modality
#define CONTROLLER_MODALITY_TYPE_UNKNOW                      (uint8_t) 0
#define CONTROLLER_MODALITY_TYPE_POWERED                     (uint8_t) 1
#define CONTROLLER_MODALITY_TYPE_ONDULATION                  (uint8_t) 2
#define CONTROLLER_MODALITY_TYPE_VIBRATION                   (uint8_t) 3
#define CONTROLLER_MODALITY_TYPE_CONF_MAX                    CONTROLLER_MODALITY_TYPE_VIBRATION
#define CONTROLLER_MODALITY_TYPE_CONF_MIN                    CONTROLLER_MODALITY_TYPE_ONDULATION
#define CONTROLLER_MODALITY_TYPE_DEFAULT                     CONTROLLER_MODALITY_TYPE_ONDULATION
#define CONTROLLER_MODALITY_TYPE_RQT_DEFAULT                 CONTROLLER_MODALITY_TYPE_UNKNOW

// Intensity
#define CONTROLLER_MODALITY_INTENSITY_0                       (uint8_t) 0
#define CONTROLLER_MODALITY_INTENSITY_1                       (uint8_t) 1
#define CONTROLLER_MODALITY_INTENSITY_2                       (uint8_t) 2
#define CONTROLLER_MODALITY_INTENSITY_3                       (uint8_t) 3
#define CONTROLLER_MODALITY_INTENSITY_4                       (uint8_t) 4
#define CONTROLLER_MODALITY_INTENSITY_5                       (uint8_t) 5
#define CONTROLLER_MODALITY_INTENSITY_CONF_MIN                CONTROLLER_MODALITY_INTENSITY_0
#define CONTROLLER_MODALITY_INTENSITY_CONF_MAX                CONTROLLER_MODALITY_INTENSITY_5
#define CONTROLLER_MODALITY_INTENSITY_DEFAULT                 CONTROLLER_MODALITY_INTENSITY_CONF_MIN
#define CONTROLLER_MODALITY_INTENSITY_RQT_DEFAULT             CONTROLLER_MODALITY_INTENSITY_CONF_MIN

//Time
#define CONTROLLER_MODALITY_TIME_SEC_0                        (uint8_t) 0
#define CONTROLLER_MODALITY_TIME_SEC_CONF_MAX                 (uint8_t) 30
#define CONTROLLER_MODALITY_TIME_SEC_CONF_MIN                 (uint8_t) 5
#define CONTROLLER_MODALITY_TIME_DEFAULT                      CONTROLLER_MODALITY_TIME_SEC_CONF_MAX
#define CONTROLLER_MODALITY_TIME_RQT_DEFAULT                  CONTROLLER_MODALITY_TIME_SEC_0
#define CONTROLLER_MODALITY_ELAPSED_TIME_DEFAULT              CONTROLLER_MODALITY_TIME_SEC_0


//Status
#define CONTROLLER_MODALITY_STATUS_UNKNOWN                   0x00
#define CONTROLLER_MODALITY_STATUS_OK                        0x01
#define CONTROLLER_MODALITY_STATUS_ERROR_1                   0x02
#define CONTROLLER_MODALITY_STATUS_DEFAULT                   CONTROLLER_MODALITY_STATUS_UNKNOWN

// Modality Request
#define CONTROLLER_MODALITY_RQT_SOURCE_UNKNOWN                 0x00
#define CONTROLLER_MODALITY_RQT_SOURCE_INT                     0x01        //internal request: buttons
#define CONTROLLER_MODALITY_RQT_SOURCE_EXT                     0x02        //external request: donnected device
#define CONTROLLER_MODALITY_RQT_SOURCE_DEFAULT                 CONTROLLER_MODALITY_RQT_SOURCE_UNKNOWN


#define CONTROLLER_MODALITY_RQT_STATUS_UNKNOWN               0x00
#define CONTROLLER_MODALITY_RQT_STATUS_OK                    0x01
#define CONTROLLER_MODALITY_RQT_STATUS_ERROR_1               0x02
#define CONTROLLER_MODALITY_RQT_STATUS_ERROR_2               0x03
#define CONTROLLER_MODALITY_RQT_STATUS_DEFAULT               CONTROLLER_MODALITY_RQT_STATUS_UNKNOWN


// -----------------------------------------------------------------------------
// Public type definitions

typedef uint8_t controller_status_t;

// -----------------------------------------------------------------------------
// Public variables
// -----------------------------------------------------------------------------
// Public function declarations


void controller_init(void);
void control_process_action(void);
void control_modality_stop(void);


uint8_t controller_modality_type_get(void);
uint8_t controller_modality_intensity_get(void);
uint8_t controller_modality_time_get(void);
uint8_t controller_modality_elapsed_time_get(void);
uint8_t controller_modality_status_get(void);

void controller_modality_type_set(uint8_t type);
void controller_modality_intensity_set(uint8_t intensity);
void controller_modality_time_set(uint8_t time);
void controller_modality_status_set(uint8_t status);
void controller_modality_set(uint8_t time, uint8_t intensity, uint8_t type, uint8_t status);

//MODALITY RQT  (request to validate)
uint8_t controller_modality_type_rqt_get(void);
uint8_t controller_modality_intensity_rqt_get(void);
uint8_t controller_modality_time_rqt_get(void);

uint8_t controller_modality_rqt_source_get(void);
uint8_t controller_modality_rqt_status_get(void);

controller_status_t controller_modality_rqt_check(uint8_t type_rqt, uint8_t intensity_rqt, uint8_t time_rqt);
void controller_modality_rqt_set(uint8_t type_rqt, uint8_t intensity_rqt, uint8_t time_rqt, uint8_t rqt_source);
void controller_modality_rqt_status_set(uint8_t rqt_status);

void controller_modality_rqt_send(void);
uint8_t controller_modality_rqt_activity_status_get(void);
void controller_modality_rqt_activity_status_set(uint8_t new_status);
void controller_modality_rqt_activity(void);


#endif /* CUSTOM_CONTROLLER_H_ */
