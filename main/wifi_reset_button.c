#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "tasks_common.h"
#include "wifi_app.h"
#include "wifi_reset_button.h"

static const char TAG[] = "wifi_reset_button";

//Semaphore handle for wifi reset button
SemaphoreHandle_t wifi_reset_semaphore = NULL;


/**
 * ISR handler for the wifi reset button
 * @param arg is a parameter passed to the ISR handler
 */
void IRAM_ATTR wifi_reset_button_isr_handler(void *arg){
    //Give the semaphore to the wifi reset button task
    xSemaphoreGiveFromISR(wifi_reset_semaphore, NULL);
}



/**
* WiFi reset button task reacts to a BOOT button event by sending a message
* to the wifi application to disconnect from Wifi and clear the stored credentials.
* @param pvParam is a parameter passed to the task
*/

void wifi_reset_button_task(void *pvParam){
    ESP_LOGI(TAG, "Wifi reset button task started");
    for(;;){
        //Wait for the semaphore to be given
        if(xSemaphoreTake(wifi_reset_semaphore, portMAX_DELAY) == pdTRUE){
            ESP_LOGI(TAG, "Wifi reset button interrupt ocurred");
            //Send a message to the wifi application to disconnect from wifi and clear the stored credentials
            wifi_app_send_message(WIFI_APP_MSG_USER_REQUESTED_STA_DISCONNECT);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
    }
}

void wifi_reset_button_config(void){
    //create semaphore
    wifi_reset_semaphore = xSemaphoreCreateBinary();

    //configure gpio
    gpio_pad_select_gpio(WIFI_RESET_BUTTON);
    gpio_set_direction(WIFI_RESET_BUTTON, GPIO_MODE_INPUT);

    //Enable interrupt on falling edge
    gpio_set_intr_type(WIFI_RESET_BUTTON, GPIO_INTR_NEGEDGE);

    //Create the Wifi reset button task
    xTaskCreatePinnedToCore(&wifi_reset_button_task, "wifi_reset_button", WIFI_RESET_BUTTON_TASK_STACK_SIZE, 
                            NULL, WIFI_RESET_BUTTON_TASK_PRIORITY, NULL, WIFI_RESET_BUTTON_TASK_CORE_ID);
    
    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    //attach the interrupt service routine
    gpio_isr_handler_add(WIFI_RESET_BUTTON, wifi_reset_button_isr_handler, NULL);

}