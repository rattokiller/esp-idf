/*
 * shared_key.c
 *
 *  Created on: Nov 19, 2021
 *      Author: Alessandro Iori
 */

// -----------------------------------------------------------------------------
// Includes

#include <stdlib.h>
#include "esp_log.h"

#define GATTS_TAG "shared_key"
//#include "sl_status.h"
//#include "sl_bt_api.h"

#include "common.h"

#include "shared_key.h"

// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------
// Private macros

#define SHARED_KEY_MEMORY_CELL                0x4001

// -----------------------------------------------------------------------------
// Private type definitions
// -----------------------------------------------------------------------------
// Private variables
uint8_t _shared_key[SHARED_KEY_LEN] = SHARED_KEY_DEFAULT_VALUE;
shared_key_presence_t _shared_key_presence = SHARED_KEY_PRESENCE_FALSE; // shared key was write one time on memory

// -----------------------------------------------------------------------------
// Private function declarations
// -----------------------------------------------------------------------------
// Private function definitions
// -----------------------------------------------------------------------------
// Public function definitions

shared_key_status_t shared_key_init(void)
{
	/*
	uint32_t sc_shared_key;
  size_t shared_key_len_result;

  uint8_t shared_key[SHARED_KEY_LEN];


  sc_shared_key = sl_bt_nvm_load((uint16_t)SHARED_KEY_MEMORY_CELL,
                                sizeof(uint8_t)*SHARED_KEY_LEN,
                                &shared_key_len_result,
                                shared_key);

  ESP_LOGI(GATTS_TAG,"shared key: %d - %02x%02x%02x%02x\r\n", shared_key_len_result,
               shared_key[0], shared_key[1], shared_key[2], shared_key[3]);


  if (sc_shared_key == SL_STATUS_OK && shared_key_len_result == SHARED_KEY_LEN)
    {
      memcpy(_shared_key, shared_key, SHARED_KEY_LEN);
      _shared_key_presence = SHARED_KEY_PRESENCE_TRUE;
      return SHARED_KEY_STATUS_TRUE;
    }
  else
    {
      return SHARED_KEY_STATUS_FALSE;
    }
  */
  return 0;
}

shared_key_status_t shared_key_erease(void)
{
	/*
  sl_status_t sc_shared_key = sl_bt_nvm_erase((uint16_t)SHARED_KEY_MEMORY_CELL);

  if(sc_shared_key == SL_STATUS_OK)
    {
      _shared_key_presence = SHARED_KEY_PRESENCE_FALSE;
      return SHARED_KEY_STATUS_TRUE;
    }
  else
    {
        return SHARED_KEY_STATUS_FALSE;
    }
    */
	return 0;
}

shared_key_presence_t shared_key_presence_get(void)
{
	return 32;
  //return _shared_key_presence;
}

shared_key_status_t shared_key_activity(uint8_t* data, uint8_t len)
{
	/*
  if(len != SHARED_KEY_LEN)
    {
      return SHARED_KEY_STATUS_FALSE;
    }

  uint8_t shared_key[SHARED_KEY_LEN];
  memcpy(shared_key, data, SHARED_KEY_LEN);

  if(_shared_key_presence == SHARED_KEY_PRESENCE_FALSE)
    {
      //save new sharedkey
      sl_status_t sc_sharedkey;

      sc_sharedkey = sl_bt_nvm_save((uint16_t)SHARED_KEY_MEMORY_CELL,
                                    SHARED_KEY_LEN,
                                    shared_key);

      if(sc_sharedkey == SL_STATUS_OK)
        {
          memcpy(_shared_key, shared_key, SHARED_KEY_LEN);
          _shared_key_presence = SHARED_KEY_PRESENCE_TRUE;
        } else {
            return SHARED_KEY_STATUS_FALSE;
        }

    } else {
        // check sharedkey
        if (memcmp((const char*)shared_key, (const char*)_shared_key, SHARED_KEY_LEN) != 0)
          {
            return SHARED_KEY_STATUS_FALSE;
          }
    }

  ESP_LOGI(GATTS_TAG,"shared key: %d - %02x%02x%02x%02x\r\n", len,
                 shared_key[0], shared_key[1], shared_key[2], shared_key[3]);


  return SHARED_KEY_STATUS_TRUE;
  */
	return 0;
}
