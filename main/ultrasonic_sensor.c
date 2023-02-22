#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ultrasonic.h>
#include <esp_err.h>


#include "tasks_common.h"

#include "ultrasonic_sensor.h"

static float distance;

float getDistance(void){
    return distance;
}

void ultrasonic_task(void *pvParameters){
    ultrasonic_sensor_t sensor = {
        .trigger_pin = TRIGGER_GPIO,
        .echo_pin = ECHO_GPIO
    };

    ultrasonic_init(&sensor);

    while (true){
        esp_err_t res = ultrasonic_measure(&sensor, MAX_DISTANCE_CM, &distance);
        if (res != ESP_OK){
            printf("Error %d: ", res);
            switch (res){
                case ESP_ERR_ULTRASONIC_PING:
                    printf("Cannot ping (device is in invalid state)\n");
                    break;
                case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
                    printf("Ping timeout (no device found)\n");
                    break;
                case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
                    printf("Echo timeout (i.e. distance too big)\n");
                    break;
                default:
                    printf("%s\n", esp_err_to_name(res));
            }
        }
        else
            printf("Distance: %0.04f m\n", distance);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void ultrasonic_task_start(){
    xTaskCreate(ultrasonic_task, "ultrasonic_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
}