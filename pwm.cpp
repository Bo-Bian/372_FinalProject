// Author:  Osman Syed
// Net ID:         osmansyed
// Date:04/06\2003
// Assignment: Lab 4
//





#include <Arduino.h>
#include <avr/io.h>
#include "pwm.h"



/*
void initPWMTimer1(){
//here we using OC1B so portb6
 DDRB |=(1 << PB6); //setting it b6 as output

//setting fast mode for OCr1A with variable register, mode 15
//choose TTCR (timer count regeister names from datasheet) 
//in our case wgm 10 and 11 are in TCCR1A and the rest are in TCCR1B
TCCR1A |= (1 << WGM11) | (1 << WGM10);
TCCR1B |= (1 << WGM12) | (1 << WGM13);

//choose non inverting mode
//here we use COM1B because OCR1B is what we use for duty cycle calculation 
//(because OCR1A is the frequency so we can't use that)
TCCR1A |= (1<< COM1B1); //data sheet for com1b1 and com1b0 for non inverting mode
TCCR1A &= ~(1 << COM1B0);

//setting prescaler to 8 
TCCR1B |= (1 << CS10);
TCCR1B &= ~( (1<<CS11) | (1<<CS12) );

//setting frequency using OCR1A by calculation
//freq_pwm = freq_io/ (prescaler(1 + OCR1A)) , 100Hz is freq_pwm for us

OCR1A = 1023; //gives us as same as fast 10 bit mode

//setting duty cycle to be a tenth of OCR1A , i.e ocr1a = 10%
OCR1B = 0;
}
*/



void initPWMTimer3(){ //setting up timer 3 for PWM signals
    DDRE |= (1<< PE4); //setting it e4 as output

    //setting fast mode for OCr1A with variable register, mode 15
    //choose TTCR (timer count regeister names from datasheet) 
    //in our case wgm 30, 31, 32, 33 
    TCCR3A |= (1 << WGM30) | (1 << WGM31); //for 30 and 31
    TCCR3B |= (1 << WGM32) | (1 << WGM33); //for 32 and 33

    //choose non inverting mode
    //here we use COM1B because OCR1B is what we use for duty cycle calculation
    //COM3B0 = 0
    //COM3B1 = 1
    TCCR3A |= (1 << COM3B1); //data sheet for com3b1 and com3b0 for non inverting mode
    TCCR3A &= ~(1 << COM3B0);

    //setting prescaler to 1
    TCCR3B |= (1 << CS30);
    TCCR3B &= ~( (1<<CS31) | (1<<CS32) );

    //setting frequency using OCR1A by calculation
//freq_pwm = freq_io/ (prescaler(1 + OCR1A)) , 100Hz is freq_pwm for us

OCR3A = 1023; //gives us as same as fast 10 bit mode

//setting duty cycle to be a tenth of OCR1A , i.e ocr1a = 10%
OCR3B = 0;

}



//****

void init_L293D(){
    DDRB |= (1 << PB5) | (1 << PB4); //setting direction for 1A and 2A, PB5 for 1A
}

void L293D_clockwise(){
    PORTB |= (1 << PB5);
    PORTB &= ~(1 << PB4);

  OCR3B = 1000; //80% duty cycle
}


void L293D_counter_clockwise(){
    //pin1A low and pin2A high
    PORTB &= ~(1 << PB5);
    PORTB |= (1 << PB4);

    OCR3B = 1000; //can OCR3B be a double
}

void STOP_motor (){
    OCR3B = 0;
}

/*void setTemperatureHumidityFan(float temperature){
   // float temp_error = temperature - DESIRED_TEMPERATURE;
   // float hum_error = humidity - DESIRED_HUMIDITY;
    
    // adjust duty cycle based on error
    if (temperature > 32.0) {
        // increase fan speed
        PORTB |= (1 << PB5);
    PORTB &= ~(1 << PB4);
    OCR3B = 1000; //80% duty cycle
       
    } 
    else {
        // maintain current fan speed
        OCR0B = 0; // set duty cycle to 50%
    }
    
    // output PWM signal
    //PORTD |= (1 << PORTD5); // turn on fan
}
*/

//takes value from potentiometer and changes the duty cycle to change speed and direction of motor
