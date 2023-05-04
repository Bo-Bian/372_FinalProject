// Author:  Ritik Makhija
// Net ID:   ritikmakhija
// Date:04/27\2023
// Assignment: Class Project
//



#ifndef PWM_H
#define PWM_H

#include <avr/io.h>

void initTemperatureHumidityPWM();

void setTemperatureHumidityFan(float temperature, float humidity);

void clearTemperatureHumidityFan();

#endif
