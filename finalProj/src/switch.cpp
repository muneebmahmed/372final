// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

void initSwitches(){
  initSwitchPB5();
  initSwitchPK0();
  initSwitchPJ1();
}

/*
 * Initializes pull-up resistor on PB3(Pin 50) and sets it into input mode
 */
void initSwitchPB5(){
  // enable pin-change interrupts
  DDRB &= ~(1 << DDB5);
  PORTB |= (1 << PORTB5);

  // enable interrupts on PB3
  PCICR |= (1 << PCIE0); //enable PCINT for pins 7-0
  PCMSK0 |= (1 << PCINT5); //enable PCINT for PCINT3
}

void initSwitchPJ1(){
  DDRJ &= ~(1 << DDJ1);
  PORTJ |= (1 << PORTJ1);

  PCICR |= (1 << PCIE1); //enable PCINT for pins 8-15
  PCMSK1 |= (1 << PCINT10); //enable PCINT for PCINT8
}

void initSwitchPK0(){
  DDRK &= ~(1 << DDK0);
  PORTK |= (1 << PORTK0);

  PCICR |= (1 << PCIE2); //enable PCINT for pins 23-16
  PCMSK2 |= (1 << PCINT16); //enable PCINT for PCINT16
}
