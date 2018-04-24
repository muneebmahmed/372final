#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi.h"

void initSPI(){
  //set SS, MOSI, and SCK to outputs
  //DDRB |= (1 << DDB2) | (1 << DDB0) | (1 << DDB1);
  //nano code below:
  DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
  // DDRL |= (1 << DDL0) | (1 << DDL2) | (1 << DDL4);

  //setting SPI prescalar to 128, SPI to master mode
  SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 <<SPR0);

  //slave select to high?
  //PORTB |= (1 << PORTB0);
  //nano:
  PORTB |= (1 << PORTB2);
}

void eightBitCommand(unsigned char data){
  //PORTB &= ~(1 << PORTB0);
  PORTB &= ~(1 << PORTB2);
  SPDR = data;
  while (!(SPSR & (1 << SPIF)));  //wait for transmission complete
  //PORTB |= (1 << PORTB0);
  PORTB |= (1 << PORTB2);
  return;
}

void twoByteCommand(unsigned char data1, unsigned char data2){
  //PORTB &= ~(1 << PORTB0);
  PORTB &= ~(1 << PORTB2);
  SPDR = data1;
  while (!(SPSR & (1 << SPIF)));
  SPDR = data2;
  while (!(SPSR & (1 << SPIF)));
  //PORTB |= (1 << PORTB0);
  PORTB |= (1 << PORTB2);
}
