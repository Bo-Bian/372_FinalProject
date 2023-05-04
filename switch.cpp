
#include "switch.h"
#include <avr/io.h>


 // Initializes pull-up resistor on PB3 and sets it into input mode
 
void initSwitchPB3(){
  
DDRB &= ~(1 << DDB3); // set direction for input
PORTB |= (1 << PORTB3);  // enable the pullup resistor for stable input
PCICR |= (1 << PCIE0); //enables PCINT 0-7
PCMSK0 |= (1 << PCINT3); //enables PCINT3
}


