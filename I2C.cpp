

#include "I2C.h"
#include <avr/io.h>
#include <Arduino.h>

/*
Trigger Temperature Measurement 0xF3 No Hold master
Trigger Humidity Measurement 0xF5 No Hold master
*/
#define WRITE_BIT 0;
#define READ_BIT 1;
#define wait_for_completion while(!(TWCR & (1 << TWINT))); 
//flags when I2C transmission is completed

//I2C_status = (TWSR & 0xF8) generalized variable to determine 
//the most recent I2C operation. Masking off lower 3 bits of status register (Prescaler bits)

//initializes the I2C module for future operation
void initI2C(){
    PRR0 &= ~(1 << PRTWI); //wakes up I2C module within 2560 megaboard
    TWSR |= (1 << TWPS0); //sets prescaler power bits = 1
    TWSR &= ~(1 << TWPS1);

    TWBR = 0x6C; //bit rate register set

    TWCR |= (1 << TWINT) | (1 << TWEN); //enable two wire interface
}

//Initiates a start condition and an I2C write transmission with a given slave module
void startI2C_Trans(unsigned char SLA){
    //initiating a start condition as well as a generic trigger action: reset TWINT flag and enable
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
    wait_for_completion; // pre-defined function to wait for the slave to finish receiving data from master

    TWDR = (SLA << 1) | WRITE_BIT; //slave address + write bit (0)

    TWCR = (1 << TWINT) | (1 << TWEN); //trigger action
    wait_for_completion;
}

//sends a stop condition to stop an I2C transmission
void stopI2C_Trans(){
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); //trigger action and sends stop condition with TWSTO bit
}

//generic function to send given data into slave module through TWDR register
void write(unsigned char data){
    TWDR = data; //loads data into TWDR register

    TWCR = (1 << TWINT) |(1 << TWEN);  // trigger action
    wait_for_completion;
}

//takes given slave module address and internal memory address to directly read data from
void read_from(unsigned char SLA, unsigned char MEMADDRESS){
    startI2C_Trans(SLA); //starts transmission with given slave module

    write(MEMADDRESS); //writes to MEMADDRESS

    //trigger action and REstart condition to begin reading from internal memory address just written in
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA); 
    wait_for_completion;

    TWDR = (SLA << 1) | READ_BIT; //slave address + read bit into data register

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); //trigger action with master ACK bit to continue reading next byte
    wait_for_completion;

    TWCR = (1 << TWINT) | (1 << TWEN); //trigger action to read last byte of data 
    wait_for_completion;

    stopI2C_Trans(); //final trigger action with stop condition sent
}


//returns TWDR to get data from I2C data register 
unsigned char read_data(){
    return TWDR;
}

unsigned int NoHoldData(unsigned char command){
    unsigned int data; //2 bytes of data from sensor
    write(command);

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA); //re - start command to wait for slave ACK
    wait_for_completion; // pre-defined function to wait for the slave to finish receiving data from master

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); //master ACK bit
    wait_for_completion;

    data = (TWDR << 8); //first byte MSB of data

    TWCR = (1 << TWINT) | (1 << TWEN); (1 << TWEA); //master ACK bit
    wait_for_completion;

    data = (data | TWDR); //second byte LSB of data

    return data;
}
