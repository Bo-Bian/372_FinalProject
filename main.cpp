#include <avr/io.h>
#include "Arduino.h"

#include <avr/interrupt.h>
#include <stdio.h>

#include "I2C.h"
#include "pwm.h"
#include "timer.h"
#include "lcd.h"

#define TEMPMEASURE 0xE3
#define HUMIDMEASURE 0xE5
#define SOFT_RESET 0xFE

#define SLA 0x40

#define Th_Temp 32

int main(){
  Serial.begin(9600); // using serial port to print values from I2C bus
  initI2C();  // initialize I2C and set bit rate
  //initSwitchPB3();
  initLCD();
  initTimer0();
  initTimer1();

  sei();
  moveCursor(0,0);
  


  startI2C_Trans(SLA);
  write(SOFT_RESET); //implementing a soft reset before taking any readings from the sensor
  stopI2C_Trans();
  
  unsigned int tempReading;
  unsigned int humidReading;
  float tempOut;
  float humidOut;

  
  initPWMTimer3();
  while(1){
    //initPWMTimer3();

    tempReading = HoldCommunication(SLA, TEMPMEASURE);
    tempReading = (tempReading & 0xFFFC);

    //Serial.println(tempReading, HEX);

    tempOut = ((((float)tempReading)/(65536.0)) * 175.72) - 46.85;
    Serial.print("Temp: ");
    Serial.println(tempOut);
    
    //_delay_ms(1000);

    humidReading = HoldCommunication(SLA, HUMIDMEASURE);
    humidReading = (humidReading & 0xFFFC);
   
    //Serial.println(humidReading, HEX);
    humidOut = ((((float)humidReading)/(65536.0)) * 125.0) - 6.0;
    Serial.print("RH: ");
    Serial.println(humidOut);

    
   if(tempOut > 25.0){
      L293D_clockwise();
      //_delay_ms(3000);
    }
    else if(tempOut < 25.0){
      STOP_motor();
    }
  
  _delay_ms(1000);
    //if(tempOut > 32){
    //  L293D_clockwise();
      //_delay_ms(3000);
    //}
    //else{
    // STOP_motor();
     
    //}
    Serial.println();

    
    
    //_delay_ms(500);

  }

  return 0;
}
