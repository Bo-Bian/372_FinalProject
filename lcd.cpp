

#include "lcd.h"
#include "timer.h"
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>
#include "Arduino.h"

// Initializes all pins related to the LCD to be outputs
void initLCDPins(){
DDRA |= (1 << DDA0) | (1<< DDA1) | (1 << DDA2) | (1 << DDA3);
DDRB |= (1 << DDB4) | (1 << DDB6);
}

/* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *      a. data is an unsigned char which has 8 bits. Therefore, you
 *         need assign the bottom 4 bits of "data" to the appropriate bits in
 *         PORTA
 *  2. This is a "command" signal, meaning RS should be Low
 *  3. Assert high on enable pin, delay, and asset low on enable pin
 *  4. delay the provided number in MICROseconds.
 */
void fourBitCommandWithDelay(unsigned char data, unsigned int delay){


// setting correct prots of PORTA high
PORTA = ((PORTA & 0XF0) | (data & 0x0F)); 
// here we will use PORTB6 for enable and then PORTB4 for RS
// 1. Set the RS pin low
PORTB &= ~(1 << PORTB6); 

// 2. set the dotaon the PORTA bus
// Use ports A0:A3 for data
// use bit masking for appending our data to PORTA

//3. enables the LCD high
PORTB |= (1 << PORTB4);

//4. delay for 1 us
delayUs(1);

// 5. put enable pin low
PORTB &= ~(1 << PORTB4);
delayUs(delay);
}

void eightBitCommandWithDelay(unsigned char command, unsigned int delay){

 //1. set up your data on PORTA bus
 // use ports A0:A3 for data
 // use bit masking for appending the upper bits of data to PORTA
 // top 4 bits
 //PORTA = (command >> 4);
PORTA = (PORTA & 0xF0) | ((command >> 4) & 0x0F); 

//2. set RS pin low
PORTB &= ~(1 << PORTB6);

// 3. enable LCD high
PORTB |= (1 << PORTB4);
delayUs(1);//4. delays 1 Us

PORTB &= ~(1 << PORTB4);//5. pin low
delayUs(1);

//6. set data on the PORTA bus/ use ports A0:A3 for data/ use bitmasking for appending the upper bits of data to PORTA
// lower 4 bits, PORTA = command;
PORTA = (PORTA & 0xF0) | (command & 0x0F); 

// 7. enable LCD high
PORTB |= (1 << PORTB4);
delayUs(1);//8. delays 1 Us

PORTB &= ~(1 << PORTB4);//9. Sets enable pin low
delayUs(delay); // 10. delay 1us
}

void writeCharacter(unsigned char character){
 
PORTA = (PORTA & 0xF0) | ((character >> 4) & 0x0F); // 1. PORTA = (command >> 4);

PORTB |= (1 << PORTB6); //2. Sets RS to high

PORTB |= (1 << PORTB4); //3. enable LCD high
delayUs(1); // 4. delays 1 us

PORTB &= ~(1 << PORTB4); // 5. Sets enable pin low

PORTA = (PORTA & 0xF0) | (character & 0x0F); // 6. lower 4 bits, PORTA = characters;

PORTB |= (1 << PORTB4);// 7. enable LCD high
delayUs(1); // 8. delays 1 us

PORTB &= ~(1 << PORTB4); // 9. Sets enable pin low

delayUs(46); // 10. do the command specified delay
}

void writeString(const char *string){

//int i = 0;

while (*string != '\0'){
writeCharacter(*string);
string++;
}
}

 // This moves the LCD cursor to a specific place on the screen.
void moveCursor(unsigned char x, unsigned char y){
unsigned char command;

x = (x << 6);

command = (0x80 | x | y);
eightBitCommandWithDelay(command, 40);
}

void initLCDProcedure(){
delayMs(15);

  // Write 0b0011 to DB[7:4] and delay 4100 microseconds
  fourBitCommandWithDelay(0b0011, 4100);
  
  // Write 0b0011 to DB[7:4] and delay 100 microseconds
  fourBitCommandWithDelay(0b0011, 100);
  
  // 0b0011 to DB[7:4] and 100us delay
  fourBitCommandWithDelay(0b0011, 100);
  
  // 0b0010 to DB[7:4] and 100us delay.
  fourBitCommandWithDelay(0b0010, 100);
  
  // Function set in the command table with 53us delay
  eightBitCommandWithDelay(0b00101000, 53); 
  
  // Display off in the command table with 53us delay
  eightBitCommandWithDelay(0b00001000, 53);

  // Display off in the command table with 3000us delay
  eightBitCommandWithDelay(0b00000001, 3000);

  // Entry Mode Set in the command table.
  eightBitCommandWithDelay(0b00000110, 53);

  // Display ON/OFF Control in the command table. 
  eightBitCommandWithDelay(0b00001110, 53);
}
void initLCD(){
  initLCDPins();
  initLCDProcedure();
}