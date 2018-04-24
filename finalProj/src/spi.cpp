#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi.h"
#include "timer.h"

void initSPI(){
  //set SS, MOSI, and SCK to outputs
  DDRB &= ~((1 << DDB2) | (1 << DDB0) | (1 << DDB1));
  DDRB |= (1 << DDB3);

  //setting SPI prescalar to 128, SPI to master mode
  SPCR |= (1 << SPE) | (1 << SPR1) | (1 <<SPR0);

  //slave select to high
  //PORTB |= (1 << PORTB0);

}

// void eightBitCommand(unsigned char data){
//   PORTL |= (1 << PORTL0) | (1 << PORTL2);
//   PORTB &= ~(1 << PORTB0);
//   SPDR = data;
//   while (!(SPSR & (1 << SPIF)));  //wait for transmission complete
//   PORTB |= (1 << PORTB0);
//   return;
// }
