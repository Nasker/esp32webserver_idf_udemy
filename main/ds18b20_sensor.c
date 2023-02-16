#include <inttypes.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ds18x20.h>
#include <esp_log.h>
#include <esp_err.h>

#include "ds18b20_sensor.h"

#define CONFIG_EXAMPLE_ONEWIRE_GPIO 25

#define CONFIG_EXAMPLE_DS18X20_ADDR 0x580417815feeff28

static const gpio_num_t SENSOR_GPIO = CONFIG_EXAMPLE_ONEWIRE_GPIO;
static const ds18x20_addr_t SENSOR_ADDR = CONFIG_EXAMPLE_DS18X20_ADDR;
static const char *TAG = "ds18x20_test";

void ds18b20_test(void *pvParameter){
    gpio_set_pull_mode(SENSOR_GPIO, GPIO_PULLUP_ONLY);
    float temperature;
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
    xTaskCreate(ds18b20_test, TAG, configMINIMAL_STACK_SIZE * 4, NULL, 5, NULL);
}
