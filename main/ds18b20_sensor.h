/*
 * ds18b20.h
 *
 *  Created on: Feb 2, 2023
 *      Author: Nasker
 */

#ifndef MAIN_DS18B20_SENSOR_H_
#define MAIN_DS18B20_SENSOR_H_


#define CONFIG_ONEWIRE_GPIO 25

#define CONFIG_DS18X20_ADDR 0x580417815feeff28

/**
 * Returns the temperature in Celsius
 */
float getTemperature(void);

/**
 * Starts the DS18B20 task
 */
void DS18B20_task_start(void);

#endif /* MAIN_DS18B20_SENSOR_H_ */
