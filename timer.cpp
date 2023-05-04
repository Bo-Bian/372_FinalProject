// Author:  Ritik Makhija
// Net ID:   ritikmakhija
// Date:04/27\2023
// Assignment: Class Project
//






#include "timer.h"

//initializing timer1 for temperature and humidity sensor
void initTemperatureHumidityTimer(){
    //setting timer to CTC mode as per datasheet
    TCCR1A  &= ~ ( (1 << WGM10) | ( 1 << WGM11));
    TCCR1B  |=  (1 << WGM12);
    TCCR1B  &=  ~(1 << WGM13);

    //Setting prescaler to 64 as per datasheet
    TCCR1B &= ~(1 << CS12);
    TCCR1B |= (1 << CS11) | (1 << CS10);

    //so OCR1A value is = 1249 using formula: [ (Td * freq / prescaler) - 1 ]:
    //so we code to set output compare match
    OCR1AL = 0x04; //upper half bits of 1249
    OCR1AH = 0xE1; //lower half bits of 1249
}

void delaySeconds(unsigned int delay){
    for(unsigned int i = 0; i < delay; ++i){
        OCR1AH = 0x30;
        OCR1AL = 0x08;

        TIFR1 |= (1 << OCF1A);

        TCNT1 = 0;

        TCCR1B &= ~((1 << CS11));
        TCCR1B |= (1 << CS10);
        TCCR1B |= (1 << CS12);

        while (!(TIFR1 & (1 << OCF1A)));
    }

    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}


