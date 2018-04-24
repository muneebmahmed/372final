#include "adc.h"
#include <avr/io.h>

void initADC7(){
  //set voltage references to be AVCC
  ADMUX |= (1 << REFS0);
  ADMUX &= ~(1 << REFS1);

  //ADLAR = 0
  ADMUX &= ~(1 << ADLAR);

  //set ADC7 as a single ended input with MUX[5:0] = 0b000111
  ADMUX |= (1 << MUX2) | (1 << MUX1) | (MUX0);
  //ADMUX &= ~((1 << MUX4) | (1 << MUX3));
  //ADCSRB &= ~(1 << MUX5);

  //set trigger to Timer/Counter 1 Overflow ADTS[2:0] = 0b110
  //ADCSRB |= (1 << ADTS0) | (1 << ADTS1);
  //ADCSRB &= ~(1 << ADTS2);
  ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));

  //originally this also turned on auto-triggering, triggering needs to be based on Timer1, and turn-on ADC
  ADCSRA |= (1 << ADEN);

  //set the pre-scalar to 128
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  //disable ADC0, ADC1, & ADC2 pin digital input
  DIDR0 |= (1 << ADC0D) | (1 << ADC1D) | (1 << ADC2D);

  //start the first conversion
  ADCSRA |= (1 << ADSC);
}

bool isAcceleration(unsigned int x, unsigned int y, unsigned int z){
  return (x*x + y*y + z*z > THRESHOLD * THRESHOLD);
}

void enablePin(unsigned char pin){
  ADMUX &= ~(1 << MUX2 | 1 << MUX1 | 1 << MUX0);  //read from ADC0
  switch (pin){
    case 2:
      ADMUX |= (1 << MUX0);     //will read from ADC1
      break;
    case 3:
      ADMUX |= (1 << MUX1);     //will read from ADC2
      break;
    default:
      break;
  }
}

unsigned int getSampledValues(){
  //return ((unsigned int)(ADCH & 3)) << 8 | ADCL;
  ADCSRA |= (1 << ADSC);//starts the conversion
  while (ADCSRA & (1 << ADSC)); //make sure conversion is complete
  unsigned int val = 0;
  val = ADCL;
  val += ((unsigned int)(ADCH) << 8);
  return val;
}
