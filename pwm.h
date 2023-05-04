// Author:  Osman Syed
// Net ID:         osmansyed
// Date:04/06\2003
// Assignment: Lab 4
//


#ifndef PWM_H
#define PWM_H

void initPWMTimer1();//this is from example code. Don't need it
void servoinit(); //timer
void initPWMTimer3(); //needed as per lab requirements
void init_L293D();
void L293D_clockwise();
void L293D_counter_clockwise();
void STOP_motor ();
void changeDutyCycle(unsigned int result);


void setTemperatureHumidityFan(float temperature);

#endif