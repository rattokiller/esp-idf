/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/****************************************************************************
*
* This demo showcases BLE GATT server. It can send adv data, be connected by client.
* Run the gatt_client demo, the client demo will automatically connect to the gatt_server demo.
* Client demo will enable gatt_server's notify after connection. The two devices will then exchange
* data.
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "freertos/queue.h"
#include "driver/gpio.h"

#include "sdkconfig.h"

static char count=0;
#include "BLE/ble_config.h"

#include "BLE/ble_functions.h"

#define GPIO_INPUT_IO_0     0
#define GPIO_INPUT_IO_1     2
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) | (1ULL<<GPIO_INPUT_IO_1))
#define ESP_INTR_FLAG_DEFAULT 0

static xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

esp_gatt_rsp_t rsp;

static void gpio_task_example(void* arg)
{
    uint32_t io_num;



    for(;;) {
        vTaskDelay(1);
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            count +=gpio_get_level(0);



            memset(&rsp, 0, sizeof(esp_gatt_rsp_t));


            if( ble_connect.param){

                ESP_LOGI(GATTS_TAG,"%s GPIO[%d] inoltro, count: %d \n",__func__, io_num,count);

                rsp.attr_value.handle = ble_connect.param->read.handle;
                rsp.attr_value.len = 20;
                rsp.attr_value.value[0] = 0x01;
                rsp.attr_value.value[1] = 0x01;

                gatts_TX.attr_value[2]= count;
                rsp.attr_value.value[2] = count;

                rsp.attr_value.value[3] = 0xff;

                esp_ble_gatts_set_attr_value(gl_profile_tab[PROFILE_A_APP_ID].char_handle_TX, //deve essere attribute hendeler
                                            20,
                                            gatts_TX.attr_value
                                            );

                esp_ble_gatts_send_indicate(ble_connect.gatts_if,
                                            ble_connect.param->read.conn_id,
                                            gl_profile_tab[PROFILE_A_APP_ID].char_handle_TX,
                                            20,
                                            gatts_TX.attr_value,
                                            true);


                esp_ble_gatts_set_attr_value(gl_profile_tab[PROFILE_A_APP_ID].char_handle_RX, //deve essere attribute hendeler
                                                            3,
                                                            gatts_TX.attr_value
                                                            );


                esp_ble_gatts_send_indicate(ble_connect.gatts_if,
									ble_connect.param->read.conn_id,
									gl_profile_tab[PROFILE_A_APP_ID].char_handle_RX,
									3,
									gatts_TX.attr_value,
									true);

            }

             ESP_LOGI(GATTS_TAG,"%s GPIO[%d] intr, val: %d , count: %d \n",__func__, io_num, gpio_get_level(0),count);
        }
    }
}

void pin_input_init(void)
{
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};

    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    ESP_LOGW(GATTS_TAG,"%s configuro...",__func__);
    gpio_config(&io_conf);

    ESP_LOGW(GATTS_TAG,"%s cambio interupt...",__func__);
    //change gpio intrrupt type for one pin
    gpio_set_intr_type(GPIO_INPUT_IO_0, GPIO_INTR_ANYEDGE);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);


    ESP_LOGW(GATTS_TAG,"%s service...",__func__);
    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_1, gpio_isr_handler, (void*) GPIO_INPUT_IO_1);

    ESP_LOGW(GATTS_TAG,"%s remove...",__func__);
    //remove isr handler for gpio number.
    gpio_isr_handler_remove(GPIO_INPUT_IO_0);
    //hook isr handler for specific gpio pin again
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);

    ESP_LOGI(GATTS_TAG," %s Minimum free heap size: %d bytes\n",__func__, esp_get_minimum_free_heap_size());

}

void app_main(void)
{
    esp_err_t ret;



    // Initialize NVS.
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    pin_input_init();

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(GATTS_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret) {
        ESP_LOGE(GATTS_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(GATTS_TAG, "%s init bluetooth failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(GATTS_TAG, "%s enable bluetooth failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_ble_gatts_register_callback(gatts_event_handler);
    if (ret){
        ESP_LOGE(GATTS_TAG, "gatts register error, error code = %x", ret);
        return;
    }
    ret = esp_ble_gap_register_callback(gap_event_handler);
    if (ret){
        ESP_LOGE(GATTS_TAG, "gap register error, error code = %x", ret);
        return;
    }

    ret = esp_ble_gatts_app_register(PROFILE_A_APP_ID);
    if (ret){
        ESP_LOGE(GATTS_TAG, "gatts app register error, error code = %x", ret);
        return;
    }

    esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(500);
    if (local_mtu_ret){
        ESP_LOGE(GATTS_TAG, "set local  MTU failed, error code = %x", local_mtu_ret);
    }
    ESP_LOGW(GATTS_TAG, "pippo run");

    while(1){
    	vTaskDelay(10);
    }

    return;
}
