// Author:         Garrett Vanhoy
// Net ID:         gvanhoy
// Date:           28 January 2017
// Assignment:     Lab 3
//----------------------------------------------------------------------//

#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

void initTimer0();
void initTimer2();
void delayUs(unsigned int delay);

#endif
