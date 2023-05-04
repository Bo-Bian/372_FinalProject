// Author:  Ritik Makhija
// Net ID:   ritikmakhija
// Date:04/27\2023
// Assignment: Cp
//

#include <avr/io.h>
//#include "timer.h" // not sure if we need a timer for this

 

void initTemperatureHumidityPWM(){
    DDRD |= (1 << DDD5); // using pin 5 as output
    PORTD &= ~(1 << PORTD5);

    // set phase correct PWM mode, 8-bit mode, prescaler 64
    TCCR0A |= (1 << WGM00) | (1 << COM0B1);
    TCCR0B |= (1 << CS01) | (1 << CS00);
    
    OCR0B = 0; // initial duty cycle of 0%
}
void setTemperatureHumidityFan(float temperature, float humidity){
    float temp_error = temperature - DESIRED_TEMPERATURE;
    float hum_error = humidity - DESIRED_HUMIDITY;
    
    // adjust duty cycle based on error
    if (temp_error > TEMP_THRESHOLD || hum_error > HUM_THRESHOLD) {
        // increase fan speed
        OCR0B = 255; // set duty cycle to 100%
    } else if (temp_error < -TEMP_THRESHOLD && hum_error < -HUM_THRESHOLD) {
        // decrease fan speed
        OCR0B = 64; // set duty cycle to 25%
    } else {
        // maintain current fan speed
        OCR0B = 128; // set duty cycle to 50%
    }
    
    // output PWM signal
    PORTD |= (1 << PORTD5); // turn on fan
}

void clearTemperatureHumidityFan(){
    OCR0B = 0; // set duty cycle to 0%
    PORTD &= ~(1 << PORTD5); // turn off fan
}


