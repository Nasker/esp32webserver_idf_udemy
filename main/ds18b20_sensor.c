#include <inttypes.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ds18x20.h>
#include <esp_log.h>
#include <esp_err.h>

#include "tasks_common.h"

#include "ds18b20_sensor.h"

static const gpio_num_t SENSOR_GPIO = CONFIG_ONEWIRE_GPIO;
static const ds18x20_addr_t SENSOR_ADDR = CONFIG_DS18X20_ADDR;
static const char *TAG = "ds18x20_test";

float temperature;

float getTemperature(void){ return temperature; }

void ds18b20_task(void *pvParameter){
    gpio_set_pull_mode(SENSOR_GPIO, GPIO_PULLUP_ONLY);
    
    esp_err_t res;
    while (1){
        res = ds18x20_measure_and_read(SENSOR_GPIO, SENSOR_ADDR, &temperature);
        if (res != ESP_OK)
            ESP_LOGE(TAG, "Could not read from sensor %08" PRIx32 "%08" PRIx32 ": %d (%s)",
                    (uint32_t)(SENSOR_ADDR >> 32), (uint32_t)SENSOR_ADDR, res, esp_err_to_name(res));
        else
            ESP_LOGI(TAG, "Sensor %08" PRIx32 "%08" PRIx32 ": %.2f°C",
                    (uint32_t)(SENSOR_ADDR >> 32), (uint32_t)SENSOR_ADDR, temperature);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void DS18B20_task_start(void){
    xTaskCreatePinnedToCore(&ds18b20_task, TAG, TEMPERATURE_TASK_STACK_SIZE , NULL, TEMPERATURE_TASK_PRIORITY, NULL, TEMPERATURE_TASK_CORE_ID);
}
