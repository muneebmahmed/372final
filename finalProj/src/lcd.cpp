// Author:         Garrett Vanhoy
// Net ID:         gvanhoy
// Date:           28 January 2017
// Assignment:     Lab 3
//----------------------------------------------------------------------//
#include "lcd.h"
#include "timer.h"
#include <stdlib.h>

/*
 * Initializes all pins related to the LCD to be outputs
 */
void initLCDPins(){
  DDRA |= (0xF << DDA0);
  DDRB |= (1 << DDB4) | (1 << DDB6);
  //DDRB |= (5 << DDB4);  //B4 = enable, B6 = RS

}

/* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *      a. data is an unsigned char which has 8 bits. Therefore, you
 *         need assign the bottom 4 bits of "data" to the appropriate bits in
 *         PORTA
 *  2. This is a "command" signal, meaning RS should be Low
 *  3. Assert high on enable pin, delay, and asset low on enable pin
 *  4. delay the provided number in MICROseconds.
 */
void fourBitCommandWithDelay(unsigned char data, unsigned int delay){
  PORTA ^= (0x0F & (PORTA ^ data));
  PORTB &= ~(1 << PORTB6);    //set RS low
  PORTB |= (1 << PORTB4);     //enable high
  delayUs(1);                 //delay
  PORTB &= ~(1 << PORTB4);    //enable low
  delayUs(delay);

}

/* Similar to fourBitCommandWithDelay except that now all eight bits of command are
 * sent.
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *       a. unlike fourBitCommandWithDelay, you need to send the TOP four bits of
 *          "command" first. These should be assigned to appropriate bits in PORTA
 * 2. This is a command signal, meaning RS should be low
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of command to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay the provided number in MICROseconds.
 */
void eightBitCommandWithDelay(unsigned char command, unsigned int delay){

  PORTA = (PORTA & 0xF0) | (command >> 4);  //upper four bits
  PORTB &= ~(1 << PORTB6);    //set RS low
  PORTB |= (1 << PORTB4);     //enable high
  delayUs(1);
  PORTB &= ~(1 << PORTB4);    //enable low
  PORTA ^= (0x0F & (PORTA ^ command));  //set lower four bits
  PORTB |= (1 << PORTB4);     //enable high
  delayUs(1);
  PORTB &= ~(1 << PORTB4);    //enable low
  delayUs(delay);
}

/* Similar to eightBitCommandWithDelay except that now RS should be high
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 * 2. This is a "data" signal, meaning RS should be high
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of character to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay is always 46 MICROseconds for a character write
 */
void writeCharacter(unsigned char character){
  PORTA = (0xF0 & PORTA) | (character >> 4);  //upper four bits
  PORTB |= (1 << PORTB6);     //set RS high
  PORTB |= (1 << PORTB4);     //enable high
  delayUs(1);
  PORTB &= ~(1 << PORTB4);    //enable low
  PORTA ^= (0x0F & (PORTA ^ character));  //set lower four bits
  PORTB |= (1 << PORTB4);     //enable high
  delayUs(1);
  PORTB &= ~(1 << PORTB4);    //enable low
  delayUs(46);
}

/*
 * Writes a provided string such as "Hello!" to the LCD screen. You should
 * remember that a c string always ends with the '\0' character and
 * that this should just call writeCharacter multiple times.
 */
void writeString(const char *string){
  while (*string != '\0'){
    writeCharacter(*string++);
  }
}

/*
 * This moves the LCD cursor to a specific place on the screen.
 * This can be done using the eightBitCommandWithDelay with correct arguments
 */
void moveCursor(unsigned char x, unsigned char y){
  unsigned char addr = (1 << 7) | (x << 6) | (y & 0xF);  //in case y > 15
  eightBitCommandWithDelay(addr, 40);
}


/* This is the procedure outline on the LCD datasheet page 4 out of 9.
 * This should be the last function you write as it largely depends on all other
 * functions working.
 */
void initLCDProcedure(){
  // Delay 15 milliseconds
  delayUs(15000);
  // Write 0b0011 to DB[7:4] and delay 4100 microseconds
  fourBitCommandWithDelay(3, 4100);
  // Write 0b0011 to DB[7:4] and delay 100 microseconds
  fourBitCommandWithDelay(3, 100);
  // The data sheet does not make this clear, but at this point you are issuing
  // commands in two sets of four bits. You must delay after each command
  // (which is the second set of four bits) an amount specified on page 3 of
  // the data sheet.
  // write 0b0011 to DB[7:4] and no delay
  fourBitCommandWithDelay(3, 0);
  // write 0b0010 to DB[7:4] and delay.
  fourBitCommandWithDelay(2, 40);

  // Function set in the command table
  eightBitCommandWithDelay(0b00101000, 40); //correct
  // Display off in the command table
  eightBitCommandWithDelay(8, 40);          //correct
  // Clear display in the command table. Remember the delay is longer!!!
  eightBitCommandWithDelay(1, 1640);        //correct
  // Entry Mode Set in the command table.
  eightBitCommandWithDelay(6, 40);          //is this 4? 6? 7?
  // Display ON/OFF Control in the command table. (Yes, this is not specified),
  // in the data sheet, but you have to do it to get this to work. Yay datasheets!)
  eightBitCommandWithDelay(15, 40);         //turn on display and cursor
}

/* Initializes Tri-state for LCD pins and calls initialization procedure.
* This function is made so that it's possible to test initLCDPins separately
* from initLCDProcedure which will likely be necessary.
*/
void initLCD(){
  initLCDPins();
  initLCDProcedure();
}

void enterAge(int age){
  char buf[4];
  initLCD();
  moveCursor(0,0);
  writeString("Enter your age");
  moveCursor(1, 4);
  writeString(itoa(age, buf, 10));
  //writeString(buf);
}
