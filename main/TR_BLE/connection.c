/*
 * connection.c
 *
 *  Created on: Nov 22, 2021
 *      Author: Alessandro Iori
 */

// -----------------------------------------------------------------------------
// Includes
#include <stdlib.h>
//#include "sl_bgapi.h"
//sostituire con nostro timere

#include "common.h"

#include "connection.h"
// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------
// Private macros

#define CONNECTION_LIST_LNG_DEFAULT     0

// -----------------------------------------------------------------------------
// Private type definitions
// -----------------------------------------------------------------------------
// Private variables

// Periodic timer handle.
//sl_simple_timer_t connection_timer;

connection_t* _connection_list;

uint8_t _connection_list_lng = CONNECTION_LIST_LNG_DEFAULT;

// -----------------------------------------------------------------------------
// Private function declarations
// -----------------------------------------------------------------------------
// Private function definitions
// -----------------------------------------------------------------------------
// Public function definitions

void connection_init(void)
{
  //_connection_list_lng = 0;
}

connection_t* connection_get_connection_list(void)
{
  return _connection_list;
}

uint8_t connection_get_connection_list_lng(void)
{
  return _connection_list_lng;
}

connection_status_t connection_add(uint8_t connection_id)
{
  connection_t *current, *prev;
  prev = (connection_t*) NULL;
  current = _connection_list;

  while(current != NULL)
    {
      if(current->id == connection_id)
        {
          //UPDATE
          current->connection = CONNECTION_STATUS_OK;
          current->notify = CONNECTION_STATUS_ERROR;
          current->auth = CONNECTION_STATUS_ERROR;
          _connection_list_lng += 1;
          return CONNECTION_STATUS_OK;
        } else {
          prev = current;
          current = current->next;
        }
    }

  //add new connection node
  connection_t* new_connection = (connection_t*) malloc(sizeof(connection_t));
  new_connection->id = connection_id;
  new_connection->connection = CONNECTION_STATUS_OK;
  new_connection->notify = CONNECTION_STATUS_ERROR;
  new_connection->auth = CONNECTION_STATUS_ERROR;
  new_connection->next = (connection_t*) NULL;

  if(prev == NULL)
    {
      _connection_list = new_connection;
    } else {
        prev->next = new_connection;
    }

  _connection_list_lng += 1;
  return CONNECTION_STATUS_OK;
}


connection_status_t connection_remove(uint8_t connection_id)
{
  connection_t* conn_to_update = connection_get(connection_id);

    if(conn_to_update == NULL)
      {
        return CONNECTION_STATUS_ERROR;
      }

    conn_to_update->connection = CONNECTION_STATUS_ERROR;
    conn_to_update->notify = CONNECTION_STATUS_ERROR;
    conn_to_update->auth = CONNECTION_STATUS_ERROR;
    _connection_list_lng -= 1;

    return CONNECTION_STATUS_OK;
}


connection_t* connection_get(uint8_t connection_id)
{
  if(_connection_list == NULL)
     {
       return (connection_t*) NULL;
     }

  connection_t *current;
  current = _connection_list;

  while((current != NULL))
     {
      if(current->id != connection_id)
        {
          current = current->next;
        } else {
          return current;
        }
     }

  return (connection_t*) NULL;
}

connection_status_t connection_status_update(uint8_t connection_id, connection_status_type_t status_type, connection_status_t new_status)
{
  connection_t* conn_to_update = connection_get(connection_id);

  if(conn_to_update == NULL)
    {
      return CONNECTION_STATUS_ERROR;
    }

  switch (status_type)
  {
    case CONNECTION_STATUS_TYPE_CONNECTION:
      //TODO: remove?
      conn_to_update->connection = new_status;
      break;
    case CONNECTION_STATUS_TYPE_AUTH:
      conn_to_update->auth = new_status;
      break;
    case CONNECTION_STATUS_TYPE_NOTIFY:
      conn_to_update->notify = new_status;
      break;
    default:
      return CONNECTION_STATUS_ERROR;
      break;
  }

  return CONNECTION_STATUS_OK;
}


//vedere il timere


// start timer
void connection_start_timer(void)
{

}

// stop timer
void connection_stop_timer(void)
{

}
