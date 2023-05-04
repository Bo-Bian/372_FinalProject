#include <avr/io.h>
#include "Arduino.h"

#include <avr/interrupt.h>
#include <stdio.h>

#include "I2C.h"
#include "switch.h"
#include "timer.h"
#include "lcd.h"
#include "pwm.h"


#define TEMPMEASURE 0xE3
#define HUMIDMEASURE 0xE5
#define SOFT_RESET 0xFE

#define SLA 0x40
#define Th_Temp 32

int main(){
  Serial.begin(9600); // using serial port to print values from I2C bus

  initSwitchPB3();//initializes PB3
  initTimer0();//initializes timer0
  initTimer1();//initializes timer1
  initLCD(); //initializes LCD
  initI2C();  // initialize I2C and set bit rate

  sei(); // Enable global interrupts.
  moveCursor(0, 0); // moves the cursor to 0,0 position
  writeString("Temp: ");
  moveCursor(1, 0);  // moves the cursor to 1,0 position
  writeString("RH: ");

  startI2C_Trans(SLA);
  write(SOFT_RESET); //implementing a soft reset before taking any readings from the sensor
  stopI2C_Trans();

  unsigned int tempReading;
  unsigned int humidReading;
  float tempOut;
  float humidOut;
  char tempPrint[6];
  char humidPrint[6];

  initPWMTimer3();
  while(1){

    delayMs(2000);
    tempReading = HoldCommunication(SLA, TEMPMEASURE);
    tempReading = (tempReading & 0xFFFC);
    //Serial.println(tempReading, HEX);

    //tempOut = 175.72 * (float(tempReading)/(65536.0)) - 46.85;
    tempOut = ((((float)tempReading)/(65536.0)) * 175.72) - 46.85;
    
    dtostrf(tempOut, 4, 2, tempPrint);
    moveCursor(0,6);
    writeString(tempPrint);

    humidReading = HoldCommunication(SLA, HUMIDMEASURE);
    humidReading = (humidReading & 0xFFFC);
    //Serial.println(humidReading, HEX);

    humidOut = ((((float)humidReading)/(65536.0)) * 125.0) - 6.0;
    //Serial.println(humidOut);
    
    dtostrf(humidOut, 4, 2, humidPrint);
    moveCursor(1,4);
    writeString(humidPrint);

    if(tempOut > Th_Temp){
      L293D_clockwise();
    }
    else {
      STOP_motor();
    }
    _delay_ms(1000);


  }

  return 0;
}