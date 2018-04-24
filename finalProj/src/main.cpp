// Author:
// Net ID:
// Date:
// Assignment:
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
// Requirements:
//----------------------------------------------------------------------//

#include <Arduino.h>
#include <avr/io.h>
#include "lcd.h"
#include "timer.h"
#include "switch.h"
#include "adc.h"
#include "spi.h"


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

typedef enum stateType_enum{
  wait_press, debounce_press, wait_release, debounce_release, no_entry,
  debounce_press2, wait_release2
} stateType;

typedef enum doubleType_enum{
  no_double, debounce, yes_double
} doubleType;

volatile stateType state = wait_press;
volatile doubleType doubleState = no_double;

volatile int age = 1;
char buf[4];

volatile unsigned int buttonIncrementCount = 0, bICthreshold = 200;

void alarm(){
  //do something that triggers an alarm
  //this just writes to the LCD for now
  //maybe have a buzzer?
  moveCursor(1,0);
  writeString("Alarm Active");
}

void initStuff(){
  // pinMode(13, OUTPUT);
  // pinMode(10, OUTPUT);
  initTimer0();
  initSwitches();
  enterAge(age);
  Serial.begin(115200);
  initTimer2();
  initADC7();
  initSPI();
  sei();
}

int main(){
  // Initialze timer, LCD, and start setup stage by asking for age first

  initStuff();
  int waitCounter = 0, bpmOut = 0, maxHeart = 0;
  while(1){

    switch(state){

      case debounce_press:
        delayUs(200); //1 millsecond
        state = wait_release;
        Serial.println("debounce_press");
        break;
      case debounce_press2:
        delayUs(200);
        state = wait_release2;
        //print?
        break;
      case debounce_release:
        delayUs(200); //1 millisecond
        state = wait_press;
        //delayUs(1000);
        Serial.println("debounce_release");
        break;
      case no_entry:
        maxHeart = 220 - age;
        maxHeart *= 0.6;
        initLCD();
        moveCursor(0,0);
        writeString("BPM: ");
        //moveCursor(1, 0);
        writeString(itoa(bpmOut, buf, 10));
        if (bpmOut > maxHeart || bpmOut < 60){
          alarm();
        }
        delayUs(100);
        break;
      case wait_press:
        //Serial.println("wait_press");
        break;
      case wait_release:
        Serial.print("wait_release: ");
        Serial.println(waitCounter++);
      default:
        break;
    }

    //delayUs(20);
    //while (!(SPSR & (1<<SPIF)));
    //Serial.println("Out of loop");
    bpmOut = SPDR;
  }

  return 0;
}


ISR(PCINT0_vect){
  Serial.println("ISR 1");
  switch (state){
    case debounce_press:
      Serial.println("Debounce press");
      break;
    case debounce_release:
      Serial.println("Debounce release");
      break;
    case wait_press:
      Serial.println("Wait Press");
      break;
    case wait_release:
      Serial.println("Wait Release");
      break;
    case wait_release2:
      Serial.println("Wait Release 2");
      break;
    case debounce_press2:
      Serial.println("Debounce Press 2");
      break;
    default:
      Serial.println("Default");
      break;
  }
  if(state == wait_press){

    state = debounce_press;
  }
  else if(state == wait_release){
    enterAge(++age);
    state = debounce_release;
  }

}

ISR(PCINT1_vect){
  Serial.println("ISR PCINT1");
  if (state == wait_press){
    state = debounce_press2;
  }
  else if (state == wait_release2){
    if (age <= 1) { age = 1; }
    enterAge(--age);
    state = debounce_release;
  }
}

ISR(PCINT2_vect){
  Serial.println("ISR 2");
  // if (doubleState == no_double){
  //   doubleState = debounce;
  // }
  // else if (doubleState == yes_double && buttonIncrementCount < 400){
  //   state = wait_press;
  //   doubleState = no_double;
  //   age = 1;
  //   return;
  // }

  if(state == wait_press){
    state = debounce_press;
  }
  else if(state == wait_release){
    initLCD();
    moveCursor(0,0);
    writeString("Upper Limit HR");
    moveCursor(1, 4);
    itoa((220 - age), buf, 10);
    writeString(buf);
    //state = debounce_release;
    state = no_entry;
  }
}

ISR(TIMER2_COMPA_vect){
  if (state == wait_release){
    if (++buttonIncrementCount == bICthreshold){
      enterAge(++age);
      buttonIncrementCount = 0;
      bICthreshold = (bICthreshold <= 100)? 100 : bICthreshold - 10;
    }
  }
  else if (state == wait_release2){
    if (++buttonIncrementCount == bICthreshold){
      enterAge(--age);
      buttonIncrementCount = 0;
    }
  }
  else{
    buttonIncrementCount = 0;
    bICthreshold = 200;
  }
  if (1) return;
}
