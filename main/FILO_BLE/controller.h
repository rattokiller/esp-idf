/*
 * tr_controller.h
 *
 *  Created on: Apr 1, 2022
 *      Author: Alessandro Iori
 */

#ifndef CUSTOM_TR_CONTROLLER_H_
#define CUSTOM_TR_CONTROLLER_H_

#include <stdlib.h>

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------
// Public macros

#define TR_CONTROLLER_STATUS_FALSE        0
#define TR_CONTROLLER_STATUS_TRUE         1

#define TR_CONTROLLER_STATUS_ERROR        0
#define TR_CONTROLLER_STATUS_OK           1

/*
 * MODALITY
 */

// Modality
#define TR_CONTROLLER_MODALITY_TYPE_UNKNOW                      (uint8_t) 0
#define TR_CONTROLLER_MODALITY_TYPE_POWERED                     (uint8_t) 1
#define TR_CONTROLLER_MODALITY_TYPE_ONDULATION                  (uint8_t) 2
#define TR_CONTROLLER_MODALITY_TYPE_VIBRATION                   (uint8_t) 3
#define TR_CONTROLLER_MODALITY_TYPE_CONF_MAX                    TR_CONTROLLER_MODALITY_TYPE_VIBRATION
#define TR_CONTROLLER_MODALITY_TYPE_CONF_MIN                    TR_CONTROLLER_MODALITY_TYPE_ONDULATION
#define TR_CONTROLLER_MODALITY_TYPE_DEFAULT                     TR_CONTROLLER_MODALITY_TYPE_ONDULATION
#define TR_CONTROLLER_MODALITY_TYPE_RQT_DEFAULT                 TR_CONTROLLER_MODALITY_TYPE_UNKNOW

// Intensity
#define TR_CONTROLLER_MODALITY_INTENSITY_0                       (uint8_t) 0
#define TR_CONTROLLER_MODALITY_INTENSITY_1                       (uint8_t) 1
#define TR_CONTROLLER_MODALITY_INTENSITY_2                       (uint8_t) 2
#define TR_CONTROLLER_MODALITY_INTENSITY_3                       (uint8_t) 3
#define TR_CONTROLLER_MODALITY_INTENSITY_4                       (uint8_t) 4
#define TR_CONTROLLER_MODALITY_INTENSITY_5                       (uint8_t) 5
#define TR_CONTROLLER_MODALITY_INTENSITY_CONF_MIN                TR_CONTROLLER_MODALITY_INTENSITY_0
#define TR_CONTROLLER_MODALITY_INTENSITY_CONF_MAX                TR_CONTROLLER_MODALITY_INTENSITY_5
#define TR_CONTROLLER_MODALITY_INTENSITY_DEFAULT                 TR_CONTROLLER_MODALITY_INTENSITY_CONF_MIN
#define TR_CONTROLLER_MODALITY_INTENSITY_RQT_DEFAULT             TR_CONTROLLER_MODALITY_INTENSITY_CONF_MIN

//Time
#define TR_CONTROLLER_MODALITY_TIME_SEC_0                        (uint8_t) 0
#define TR_CONTROLLER_MODALITY_TIME_SEC_CONF_MAX                 (uint8_t) 30
#define TR_CONTROLLER_MODALITY_TIME_SEC_CONF_MIN                 (uint8_t) 5
#define TR_CONTROLLER_MODALITY_TIME_DEFAULT                      TR_CONTROLLER_MODALITY_TIME_SEC_CONF_MAX
#define TR_CONTROLLER_MODALITY_TIME_RQT_DEFAULT                  TR_CONTROLLER_MODALITY_TIME_SEC_0
#define TR_CONTROLLER_MODALITY_ELAPSED_TIME_DEFAULT              TR_CONTROLLER_MODALITY_TIME_SEC_0


//Status
#define TR_CONTROLLER_MODALITY_STATUS_UNKNOWN                   0x00
#define TR_CONTROLLER_MODALITY_STATUS_OK                        0x01
#define TR_CONTROLLER_MODALITY_STATUS_ERROR_1                   0x02
#define TR_CONTROLLER_MODALITY_STATUS_DEFAULT                   TR_CONTROLLER_MODALITY_STATUS_UNKNOWN

// Modality Request
#define TR_CONTROLLER_MODALITY_RQT_SOURCE_UNKNOWN                 0x00
#define TR_CONTROLLER_MODALITY_RQT_SOURCE_INT                     0x01        //internal request: buttons
#define TR_CONTROLLER_MODALITY_RQT_SOURCE_EXT                     0x02        //external request: donnected device
#define TR_CONTROLLER_MODALITY_RQT_SOURCE_DEFAULT                 TR_CONTROLLER_MODALITY_RQT_SOURCE_UNKNOWN


#define TR_CONTROLLER_MODALITY_RQT_STATUS_UNKNOWN               0x00
#define TR_CONTROLLER_MODALITY_RQT_STATUS_OK                    0x01
#define TR_CONTROLLER_MODALITY_RQT_STATUS_ERROR_1               0x02
#define TR_CONTROLLER_MODALITY_RQT_STATUS_ERROR_2               0x03
#define TR_CONTROLLER_MODALITY_RQT_STATUS_DEFAULT               TR_CONTROLLER_MODALITY_RQT_STATUS_UNKNOWN


// -----------------------------------------------------------------------------
// Public type definitions

typedef uint8_t tr_controller_status_t;

// -----------------------------------------------------------------------------
// Public variables
// -----------------------------------------------------------------------------
// Public function declarations


void tr_controller_init(void);
void tr_control_process_action(void);
void tr_control_modality_stop(void);


uint8_t tr_controller_modality_type_get(void);
uint8_t tr_controller_modality_intensity_get(void);
uint8_t tr_controller_modality_time_get(void);
uint8_t tr_controller_modality_elapsed_time_get(void);
uint8_t tr_controller_modality_status_get(void);

void tr_controller_modality_type_set(uint8_t type);
void tr_controller_modality_intensity_set(uint8_t intensity);
void tr_controller_modality_time_set(uint8_t time);
void tr_controller_modality_status_set(uint8_t status);
void tr_controller_modality_set(uint8_t time, uint8_t intensity, uint8_t type, uint8_t status);

//MODALITY RQT  (request to validate)
uint8_t tr_controller_modality_type_rqt_get(void);
uint8_t tr_controller_modality_intensity_rqt_get(void);
uint8_t tr_controller_modality_time_rqt_get(void);

uint8_t tr_controller_modality_rqt_source_get(void);
uint8_t tr_controller_modality_rqt_status_get(void);

tr_controller_status_t tr_controller_modality_rqt_check(uint8_t type_rqt, uint8_t intensity_rqt, uint8_t time_rqt);
void tr_controller_modality_rqt_set(uint8_t type_rqt, uint8_t intensity_rqt, uint8_t time_rqt, uint8_t rqt_source);
void tr_controller_modality_rqt_status_set(uint8_t rqt_status);

void tr_controller_modality_rqt_send(void);
uint8_t tr_controller_modality_rqt_activity_status_get(void);
void tr_controller_modality_rqt_activity_status_set(uint8_t new_status);
void tr_controller_modality_rqt_activity(void);


#endif /* CUSTOM_TR_CONTROLLER_H_ */
