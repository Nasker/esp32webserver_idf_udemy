#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <mcp960x.h>
#include <string.h>

#include "mcp9600_sensor.h"
#include "tasks_common.h"

#define I2C_PORT 0
#define I2C_ADDR 0x60

static const char *TAG = "mcp9600_test";

static struct { gpio_num_t sda, scl; } pins = { 21, 22 };

float t;

float getThermocouple(void){
    return t;
}

static mcp960x_t* sensors;

void mcp9600_test(void *pvParameters){
    sensors = (mcp960x_t*)malloc(sizeof(mcp960x_t)); // allocate memory
    memset(sensors, 0, sizeof(mcp960x_t));
    ESP_ERROR_CHECK(mcp960x_init_desc(sensors, I2C_ADDR, I2C_PORT, pins.sda, pins.scl));
    ESP_ERROR_CHECK(mcp960x_init(sensors));
    ESP_ERROR_CHECK(mcp960x_set_device_config(sensors, MCP960X_MODE_NORMAL, MCP960X_SAMPLES_1, MCP960X_ADC_RES_18, MCP960X_TC_RES_0_0625));
    ESP_ERROR_CHECK(mcp960x_set_sensor_config(sensors, MCP960X_TYPE_K, MCP960X_FILTER_MAX));
    ESP_LOGI(TAG, "Sensor: SDA=%u, SCL=%u, HW ID 0x%02x, HW Rev. 0x%02x\n", pins.sda, pins.scl, sensors->id, sensors->revision);
    bool ready;
    mcp960x_status_t status;

    while (1){
        ESP_ERROR_CHECK(mcp960x_get_status(sensors, &ready, NULL, &status, NULL, NULL, NULL, NULL));
        if (!ready){
            printf("Not ready\n");
            continue;
        }
        ESP_ERROR_CHECK(mcp960x_get_thermocouple_temp(sensors, &t));
        ESP_LOGI(TAG, "T: %.2fºC", t);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void MCP9600_task_start(void ){
    ESP_ERROR_CHECK(i2cdev_init());
    xTaskCreate(mcp9600_test, "test", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL);
}
