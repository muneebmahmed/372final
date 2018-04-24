#ifndef ADC_H
#define ADC_H

#define THRESHOLD 6

void initADC7();
bool isAcceleration(unsigned int x, unsigned int y, unsigned int z);
void enablePin(unsigned char pin);
unsigned int getSampledValues();

#endif
