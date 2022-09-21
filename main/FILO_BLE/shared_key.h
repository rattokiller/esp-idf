/*
 * shared_key.h
 *
 *  Created on: Nov 19, 2021
 *      Author: Alessandro Iori
 */

#ifndef CUSTOM_SHARED_KEY_H_
#define CUSTOM_SHARED_KEY_H_

// Private macros

#define SHARED_KEY_STATUS_FALSE        0
#define SHARED_KEY_STATUS_TRUE         1

#define SHARED_KEY_PRESENCE_FALSE      0
#define SHARED_KEY_PRESENCE_TRUE       1

#define SHARED_KEY_LEN                        4
#define SHARED_KEY_DEFAULT_VALUE              {0xFF, 0xFF, 0xFF, 0xFF}

// -----------------------------------------------------------------------------
// Public type definitions
typedef uint8_t shared_key_status_t;
typedef uint8_t shared_key_presence_t;

// -----------------------------------------------------------------------------
// Public function declarations

shared_key_status_t shared_key_init(void);
shared_key_status_t shared_key_erease(void);
shared_key_presence_t shared_key_presence_get(void);
shared_key_status_t shared_key_activity(uint8_t* data, uint8_t len);

#endif /* CUSTOM_SHARED_KEY_H_ */
