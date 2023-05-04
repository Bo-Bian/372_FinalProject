
#include <avr/io.h>
#include "timer.h"

// initialize mode of operation for Timer1 using CTC mode
// WGM bits need configured

void initTimer1() {
TCCR1A &= ~( (1 << WGM10) | ( 1<< WGM11));
TCCR1B |= ( 1 << WGM12);
TCCR1B &= ~ ( 1 << WGM13); 
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delayUs(unsigned int delay){

unsigned int count = 0;
 OCR1A = 2;
//OCR1AH = 0x00;//sets the compare register with a value of 2
//OCR1AL = 0x02;

TCCR1B &= ~(1 << CS10) | (1 << CS12); 
TCCR1B |= (1 << CS11);


 
while (count < delay){
    
TCNT1 = 0;
TIFR1 |= (1 << OCF1A);//set flag down

// poll the flag OCF1A bit to see when it is raised
// while the flag bit OCF1A is down , do nothing
    while (!(TIFR1 & (1 << OCF1A))==0); //This executes one delay cycle, checking until the flag is raised after 1 Us

    count++;

}

// exit out we have our delay required
// turn off clock
TCCR1B &= ~( ( 1 << CS12) | ( 1 << CS11) | (1 << CS10));
}

void initTimer0(){ //Initialize timer 0

//TCCR0A and TCCR0B – configuration WGM (modes)
// CS (clock select to turn timer off and pre-scaling)
// Setting time 0 into into CTC mode by placing 0 into WGM00 & WGM02, then a 1 into WGM01
TCCR0A &= ~(1 << WGM00);
TCCR0A |= (1 << WGM01);
TCCR0B &= ~(1 << WGM02);

}

// 100 millisecond delay
void delayMs(unsigned int delay){
    unsigned int count = 0; //to compare with delay

OCR0A = 249;
// turn on the timer by setting the prescaler to 64
TCCR0B  |= (1 << CS00) | (1 << CS01);
TCCR0B  &= ~ (1 << CS02);

while (count < delay){

// set the flag down (remember to set the flag down we set a logic 1)
TIFR0 |= (1 << OCF0A);
// clear the timer
TCNT0 = 0;

// while flag is down do not do anything
while (!(TIFR0 & (1 << OCF0A)));
// exit while loop when flag is up
count++;
}
// turn clock off
TCCR0B  &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));

}