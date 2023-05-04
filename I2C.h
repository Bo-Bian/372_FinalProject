#ifndef I2C_H
#define I2C_H
void initI2C();
/*
- Wake up I2C module on mega 2560
- Set prescaler TWPS to 1
- Set two wire interface bit rate register TWBR
- Enable two wire interface
*/

void startI2C_Trans(unsigned char SLA);
/*
- Clear TWINT, initiate start condition, initiate enable
- Wait for completion
- Set two wire data register to the SLA + write bit
- Trigger action: Clear TWINT and initiate enable
- Wait for completion
*/

void stopI2C_Trans();
/*
- Trigger action + stop condition
- Write(unsigned char data)
- Set two wire data register equal to incoming data
- Trigger action
- Wait for completion
*/

void write(unsigned char data);

void read_from(unsigned char SLA, unsigned char MEMADDRESS);
/*
- Start a transmission to the SLA
- Write to the MEMADDRESS
- Clear TWINT, initiate start condition, initiate enable
- Wait for completion
- Set two wire data register to the SLA + read bit
- Trigger action + master acknowledge bit
- Wait for completion
- Trigger action
- Wait for completion
- Trigger action + stop condition
*/

unsigned char read_data();
/*
- Return TWDR
*/
unsigned char HoldCommunication(unsigned char SLA, unsigned char command);
//getting 2 bytes of data from sensor as shown in Hold Master Communication Sequence on pg.11 of datasheet


#endif