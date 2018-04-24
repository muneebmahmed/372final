/// Author:         Garrett Vanhoy
// Net ID:         gvanhoy
// Date:           28 January 2017
// Assignment:     Lab 3
//----------------------------------------------------------------------//

#include "timer.h"

/* Initialize timer 0, you should not turn the timer on here. You will need to
* use CTC mode
*/
void initTimer0(){
  TCCR0A |= (1 << WGM01);
  TCCR0A &= ~(1 << WGM00);
}

void initTimer2(){
  TCCR2A = 0x02;
  TCCR2B = 0x06;
  OCR2A = 0x7C;
  TIMSK2 = 0x02;
}

/* This delays the program an amount specified by unsigned int delay.
*/
void delayUs(unsigned int delay){
  //calculate number of ticks
  OCR0A = 2;
  // clear the timer
  TCNT0 = 0;
  // calculate the TOP value and assign it to OCR1A

  // Turn on the timer
  TCCR0B |= (1 << CS01); //pre-scaler of 8
  // Do nothing while the OCF0A flag is not up
  for(unsigned int i = 0; i < delay; i++){
    while(!(TIFR0 & (1 << OCF0A)));
    TIFR0 |= (1 << OCF0A);
  }
  // Clear the OCF0A flag

  // Turn off the timer
  TCCR0B &= ~(1 << CS02 | 1 << CS01 | 1 << CS00);
}
