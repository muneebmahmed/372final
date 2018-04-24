#include <Arduino.h>
#include <avr/io.h>
#include "spi.h"
#include "timer.h"
#include "adc.h"

int pulsePin = 0;
int blinkPin = 13;
int fadePin = 5;
int fadeRate = 0;

volatile int BPM, sig, beatInterval = 600, peak = 512, trough = 512, threshold = 512, amp = 100;
volatile boolean pulse = false, beatFound = false, firstBeat = true, secondBeat = false;
volatile int rate[10];
volatile unsigned long sampleCounter = 0, lastBeatTime = 0;

void resetVars(){
  threshold = peak = trough = 512;
  firstBeat = true;
  secondBeat = false;
  lastBeatTime = sampleCounter;
}

unsigned int calcBPM(unsigned long sum){
  return 600000/sum;
}

int main(){
  sei();
  initSPI();
  initADC7();
  initTimer2();
  enablePin(1);
  Serial.begin(115200);
  unsigned char eightBitBPM;
  while(1){
    //eightBitCommand(42);
    //eightBitCommand(72);
    delayUs(1000);
    eightBitBPM = BPM;
    if (beatFound){
      Serial.println(BPM);
      eightBitCommand(eightBitBPM);
      beatFound = false;
    }
  }
}

ISR(TIMER2_COMPA_vect){
  //if (1) return;
  //sig = analogRead(pulsePin);
  sig = getSampledValues();
  sampleCounter += 2;
  int N = sampleCounter - lastBeatTime;
  if (sig < threshold && N > (beatInterval/5)*3){
    if (sig < trough){
      trough = sig;
    }
  }
  if (sig > threshold && sig > peak){
    peak = sig;
  }
  //wait 250 milliseconds to reduce noise
  if (N > 250){
    if ((sig > threshold) && !pulse && (N > (beatInterval/5)*3)){
      pulse = true;
      //digitalWrite(pulsePin, HIGH);
      beatInterval = sampleCounter - lastBeatTime;
      lastBeatTime = sampleCounter;
      //make sure you're not on init to have proper values
      if (secondBeat){
        secondBeat = false;
        for (int i = 0; i <= 9; i++){
          rate[i] = beatInterval;
        }
      }
      if (firstBeat){
        firstBeat = false;
        secondBeat = true;
        //sei();
        return;
      }
      unsigned long intervalSum = 0;
      for (int i = 0; i <= 8; i++){
        rate[i] = rate[i+1];
        intervalSum += rate[i];
      }
      rate[9] = beatInterval;
      intervalSum += rate[9];
      BPM = 600000/intervalSum;
      beatFound = true;
    }
  }
  if (sig < threshold && pulse){
    //digitalWrite(13, LOW);
    pulse = false;
    amp = peak - trough;
    peak = trough = threshold = (amp >> 1) + trough;
    //peak = trough = threshold;
  }
  if (N > 2500){
    resetVars();
  }
}
