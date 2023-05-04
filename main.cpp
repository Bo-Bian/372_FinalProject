#include <avr/io.h>
#include "Arduino.h"

#include <avr/interrupt.h>
#include <stdio.h>

#include "I2C.h"

#define TEMPMEASURE 0xE3
#define HUMIDMEASURE 0xE5
#define SOFT_RESET 0xFE

#define SLA 0x40

int main(){
  Serial.begin(9600); // using serial port to print values from I2C bus
  initI2C();  // initialize I2C and set bit rate

  startI2C_Trans(SLA);
  write(SOFT_RESET); //implementing a soft reset before taking any readings from the sensor
  stopI2C_Trans();

  unsigned int tempReading;
  unsigned int humidReading;
  float tempOut;
  float humidOut;

  while(1){

    tempReading = HoldCommunication(SLA, TEMPMEASURE);
    tempReading = (tempReading & 0xFFFC);
    //Serial.println(tempReading, HEX);

    tempOut = ((((float)tempReading)/(65536.0)) * 175.72) - 46.85;
    //Serial.println(tempOut);

    humidReading = HoldCommunication(SLA, HUMIDMEASURE);
    humidReading = (humidReading & 0xFFFC);
    //Serial.println(humidReading, HEX);

    humidOut = ((((float)humidReading)/(65536.0)) * 125.0) - 6.0;
    Serial.println(humidOut);



  }

  return 0;
}