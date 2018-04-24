Course Name: ece372a <br>
Author: <br>
NetID: <br>
Date: <br>
Project Name: lab1

# Description
In this lab, you will use SFR Macros and bit operations to manipulate several LEDs as well as use a switch to change the rate at which the LEDs blink.

## Expected Outcome
![Lab 1 GIF](https://storage.googleapis.com/ece-gitlab/lab1.gif)

# Instructions
1. Complete led.c, led.h, switch.cpp, and switch.h to fulfill the requirements.
2. Employ proper version control practices by committing and pushing periodically. Your code will be tested based on the most recent commit after the final due date

# Requirements
## Overall
1. The program must compile. This can be tested using the checkmark icon.
2. The project must be pushed to GitLab and an appropriate response from the server must be received: "Got your submission. Thanks!"

## led.cpp
1. A function implementation that returns void and has no parameters called initLED must be present and is used in the main function to initialize all LED pins as outputs.
2. Pins 2-9 on the development board must be used to control the LEDs
3. A function implementation called turnOnLED that returns void and has a parameter called led of type unsigned unsigned int must be present.
4. The turnOnLED function must turn *on* the LED corresponding to the pin on the development board and *not affect the state of other LEDs*.
5. A function implementation called turnOffLED that returns void and has a parameter called led of type unsigned unsigned int must be present.
6. The turnOffLED function must turn *off* the LED corresponding to the pin on the development board and *not affect the state of other LEDs*.  

## switch.cpp
1. A function implementation that returns void and has no parameters called initSwitch must be present and is used in the main function to initialize *pin 32* on the development board as an input.

## main.cpp
1. A call to initLED() and initSwitch() must be present in the main function.
2. An infinite while loop must be present.
3. A call to testLED must be present in the main loop.
4. While the switch is not pressed, the LEDs must change every 100 ms and while the switch is pressed, the LEDs must change with a rate *half* as fast

## delay
The built-in function _delay_ms(unsigned int) may be used for this project to cause a delay between LEDs blinking. It is important to note that this function requires a *constant* value. Therefore, do a long delay, you can use #define LONG_DELAY 200 and _delay_ms(LONG_DELAY) for a long delay. For a short delay, you can use #define SHORT_DELAY 100 and _delay_ms(SHORT_DELAY)

# Rubric
![Lab 1](https://storage.googleapis.com/ece-gitlab/lab-rubric.png)
