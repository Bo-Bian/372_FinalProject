#include "lcd.h"
#include "timer.h"
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>
#include "Arduino.h"



/*
 * Initializes all pins related to the LCD to be outputs
 */
void initLCDPins(){

DDRA |= (1 << DDA0) | (1<< DDA1) | (1 << DDA2) | (1 << DDA3);//Datapins fro LCD display, pins 18, 19, 20, 21 

DDRB |= (1 << DDB4) | (1 << DDB6);//pin B4 is for pin enable, B6 is for RS pin 
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

//PORTA = data;

PORTA = ((PORTA & 0XF0) | (data & 0x0F));//Assigns data passed to LCD

PORTB &= ~(1 << PORTB6);//RS pin to command 

PORTB |= (1 << PORTB4);//enable pin to high

delayUs(1);//delays for a microsecond

PORTB &= ~(1 << PORTB4);//Sets enable to low

//delayUs(delay);//Delays so that command can be executed
delayUs(delay);
}

/* Similar to fourBitCommandWithDelay except that now all eight bits of command are
 * sent.
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *       a. unlike fourBitCommandWithDelay, you need to send the TOP four bits of
 *          "command" first. These should be assigned to appropriate bits in PORTA
 * 2. This is a command signal, meaning RS should be low
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of command to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay the provided number in MICROseconds.
 */
void eightBitCommandWithDelay(unsigned char command, unsigned int delay){
 
PORTA = (PORTA & 0xF0) | ((command >> 4) & 0x0F); //Assigns Port A with last 4 bits of command

//PORTA = (PORTA << 4);

PORTB &= ~(1 << PORTB6);//Sets RS pin to command 

PORTB |= (1 << PORTB4);//Sets enable pin high allowing for data to be taken in 

delayUs(1);//delays 1 Us to get information

PORTB &= ~(1 << PORTB4);//Sets enable pin low

PORTA = (PORTA & 0xF0) | (command & 0x0F); //Port A gets assigned with first 4 bits of command

//PORTA = (PORTA << 4);

PORTB |= (1 << PORTB4);//Sets enable pin high allowing for data to be taken in 

delayUs(1);//delays 1 Us to get information

PORTB &= ~(1 << PORTB4);//Sets enable pin to low


delayUs(delay);
}

/* Similar to eightBitCommandWithDelay except that now RS should be high
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 * 2. This is a "data" signal, meaning RS should be high
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of character to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay is always 46 MICROseconds for a character write
 */
void writeCharacter(unsigned char character){
 
PORTA = (PORTA & 0xF0) | ((character >> 4) & 0x0F); //PORTA equals high nibble of char

//PORTA = (PORTA << 4);

PORTB |= (1 << PORTB6);//RS to display

PORTB |= (1 << PORTB4);//enable pin to high 

delayUs(1);//delays 1 Us to get information 

PORTB &= ~(1 << PORTB4);//enable pin to low

PORTA = (PORTA & 0xF0) | (character & 0x0F); //assign port A with 4 bits of char

//PORTA = (PORTA << 4);

PORTB |= (1 << PORTB4);//enable pin to high

delayUs(1);

PORTB &= ~(1 << PORTB4);//Sets enable pin low

delayUs(46);//46 microseconds delay to execute display
}




/*
 * Writes a provided string such as "Hello!" to the LCD screen. You should
 * remember that a c string always ends with the '\0' character and
 * that this should just call writeCharacter multiple times.
 */
void writeString(const char *string){

int i = 0;

while (string[i] != '\0'){
writeCharacter(string[i]);
i++;
}


}



/*
 * This moves the LCD cursor to a specific place on the screen.
 * This can be done using the eightBitCommandWithDelay with correct arguments
 */
void moveCursor(unsigned char x, unsigned char y){
unsigned char command;

x = (x << 6);

command = (0x80 | x | y);
eightBitCommandWithDelay(command, 40);
}


/* This is the procedure outline on the LCD datasheet page 4 out of 9.
 * This should be the last function you write as it largely depends on all other
 * functions working.
 */
void initLCDProcedure(){
  // Delay 15 milliseconds
//Serial.begin(9600);
delayMs(15);
//Serial.println("I am here");
//Serial.flush();
  // Write 0b0011 to DB[7:4] and delay 4100 microseconds
  
  fourBitCommandWithDelay(0b0011, 4100);
  
  // Write 0b0011 to DB[7:4] and delay 100 microseconds

  fourBitCommandWithDelay(0b0011, 100);
  

  // The data sheet does not make this clear, but at this point you are issuing
  // commands in two sets of four bits. You must delay after each command
  // (which is the second set of four bits) an amount specified on page 3 of
  // the data sheet.
  // write 0b0011 to DB[7:4] and 100us delay

  fourBitCommandWithDelay(0b0011, 100);
  

  // write 0b0010 to DB[7:4] and 100us delay.

  fourBitCommandWithDelay(0b0010, 100);
  

  // Function set in the command table with 53us delay

  eightBitCommandWithDelay(0b00101000, 53); 
  

  // Display off in the command table with 53us delay

eightBitCommandWithDelay(0b00001000, 53);
//delayUs(53);
  // Clear display in the command table. Remember the delay is longer!!!

eightBitCommandWithDelay(0b00000001, 3000);


  // Entry Mode Set in the command table.

eightBitCommandWithDelay(0b00000110, 53);

  // Display ON/OFF Control in the command table. (Yes, this is not specified),
  // in the data sheet, but you have to do it to get this to work. Yay datasheets!)

eightBitCommandWithDelay(0b00001110, 53);
}



/* Initializes Tri-state for LCD pins and calls initialization procedure.
* This function is made so that it's possible to test initLCDPins separately
* from initLCDProcedure which will likely be necessary.
*/
void initLCD(){
  initLCDPins();
  initLCDProcedure();
}