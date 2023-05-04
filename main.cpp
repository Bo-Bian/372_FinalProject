#include <avr/io.h>
#include "Arduino.h"

#include <avr/interrupt.h>
#include <stdio.h>

#include "I2C.h"

#define TEMPMEASURE 0xE3
#define HUMIDMEASURE 0xE5

#define SLA 0x40

#define PWR_MGMT  0x6B //power mangement sleep mode
#define WAKEUP 0x00 //wake it up by writing 00 to it

int main(){
  Serial.begin(9600); // using serial port to print values from I2C bus
  initI2C();  // initialize I2C and set bit rate

  startI2C_Trans(SLA);
  write(PWR_MGMT);// address on SLA for Power Management
  write(WAKEUP); // send data to Wake up from sleep mode
  stopI2C_Trans();

  unsigned char tempReading;
  float tempOut;

  while(1){

    tempReading = HoldCommunication(SLA, TEMPMEASURE);
    tempReading = (tempReading & 0xFFFC);
    //Serial.println(tempReading, HEX);

    tempOut = ((((float)tempReading)/(65536.0)) * 175.72) - 46.85;
    Serial.println(tempOut);

  }

  return 0;
}
