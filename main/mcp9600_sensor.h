/*
 * mcp9600.h
 *
 *  Created on: Feb 13, 2023
 *      Author: Nasker
 */

#ifndef MAIN_MCP9600_SENSOR_H_
#define MAIN_MCP9600_SENSOR_H_

float getThermocouple(void);

/**
 * Starts the MCP9600 task
 */
void MCP9600_task_start(void);



#endif /* MAIN_MCP9600_SENSOR_H_ */