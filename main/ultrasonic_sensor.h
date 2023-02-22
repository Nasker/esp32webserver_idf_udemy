#ifndef __MAIN_ULTRASONIC_SENSOR_H__
#define __MAIN_ULTRASONIC_SENSOR_H__

#define MAX_DISTANCE_CM 20 // 5m max
#define TRIGGER_GPIO 19
#define ECHO_GPIO 23


float getDistance(void);

void ultrasonic_task_start(void);

#endif /* __MAIN_ULTRASONIC_SENSOR_H__ */