Lab 2: Morse Code Decoder (F25)

ESE 5190: Smart Devices

University of Pennsylvania

Deadline: Sep 29, 2025 11:59 PM

![](./Lab%202%20-%20Morse%20-%20F25_images/image-001.gif)

# Overview

Find the GitHub Classroom assignment at the link below, which includes a starter codebase for you. Don’t forget to review the [ESE5190 F25 Assignment Readme](https://docs.google.com/document/d/14to5z6nSutG9GPNAv13D4yna4YaUmD3YhLlwbTLNPXk/edit?tab=t.0) for details on submitting assignments this semester.

**All the required questions are highlighted in green.**

**Important notes are highlighted in yellow.**

A **group** submission to be submitted on GitHub Classroom & Gradescope:[ESE5190 F25 Lab Teams](https://docs.google.com/spreadsheets/d/1jNFi0udujE0_Wze6cH2xqglsPmEp42Et-MKAw4vWiSw/edit?gid=1817945493#gid=1817945493)

**GitHub Classroom Assignment:** [https://classroom.github.com/a/FUxIbFSO](https://classroom.github.com/a/FUxIbFSO)

Important: One team member should create the team first, and the other joins it.

No late days are allowed on lab submissions.

You will be working with a randomly assigned lab partner - find yours here, in the appropriate spreadsheet tab: [ESE5190 F25 Lab Teams](https://docs.google.com/spreadsheets/d/1jNFi0udujE0_Wze6cH2xqglsPmEp42Et-MKAw4vWiSw/edit?gid=1817945493#gid=1817945493). You should be working as a team, making approximately equal effort. **Work together on all parts of the lab!** A partner evaluation form will be sent out at the end of the lab, wherein each student will log their effort and their partner's efforts. So, make sure to do the work!

–

In this lab, you will design a “Morse code decoder.” This device allows you to translate a Morse code signal into its alphanumeric representation.

Morse code consists of dashes and dots. separated by spaces; different sequences of these signals correspond to an alphanumeric character. Check out the [Wikipedia article on Morse code](https://en.wikipedia.org/wiki/Morse_code) to learn more.

You will build this decoder by detecting an incoming signal coming from a push button (first with polling, then with interrupts). Then, you will display the translated message; you will light LEDs based on what signals you receive and write out the message in its ASCII form to a serial port for you to read.

You will submit your answers using the GitHub Classroom assignment and Gradescope. Since this lab requires a few video demonstrations, ensure your links work.

# Learning Objectives

After this lab, you should understand the following:

-   Build a circuit on a breadboard using buttons and LEDs
-   Work with the digital I/O ports available on a microcontroller
-   Program a microcontroller in C at the register-level
-   Implement event-based polling
-   Utilize hardware interrupts and timers for event handling
-   Write ASCII data to a serial port

# Required Equipment

## Hardware

-   ATmega328PB Xplained Board, jumper wires, breadboard
-   LEDs (x4)
-   Pushbutton (x1)
-   Resistors (10k, 1k)
-   Capacitors (1nF, 10nF)

## Software

-   MPLAB X IDE: [MPLAB X IDE Set up](https://docs.google.com/document/d/1tdrCcBpLYTH8mV46g0bIkuaSq1z6e9EHJh_3Zq-_rXU/edit?usp=drive_link)
-   Debugging on MPLAB: [Debugging with ATmega328PB XMINI](https://docs.google.com/document/d/1vowBmvEa4ni1BSvyxoxkDXFa7gcXdA-w6XgsiwTzdO0/edit?usp=drive_link)

# Equipment Tutorials

If this is your first time working with circuits and electronics, go over the following tutorials:

-   [UPenn ESE Labs How to Solder (Video)](https://www.youtube.com/watch?v=3WbDT2CA3FQ)
-   [UPenn ESE Labs Soldering Policy](https://docs.google.com/document/d/1RcKx8MTbHE_Vv_7RbFUgvzVCkegLFVyvIDUmgu71wqU/edit#heading=h.v6t1jmo3sip3)
-   [Adafruit How to Solder Guide](https://learn.adafruit.com/adafruit-guide-excellent-soldering?view=all)
-   [Understanding solderless breadboards](https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v41/understanding-breadboards) ([no more Fun and informational comic](https://electronicsarduinobreadboardlab.files.wordpress.com/2017/02/breadboardworkshop.jpg?w=768))
-   [How to use an oscilloscope](https://learn.sparkfun.com/tutorials/how-to-use-an-oscilloscope/all) (One of the most important and useful measurement tools. Make sure you know how to use the one that is in your parts kit.)
-   [How to use a multimeter](https://learn.sparkfun.com/tutorials/how-to-use-a-multimeter/all)

# 

# Part A: Warming Up with LEDs

We begin with a prelude to Lab 2.

-   It will hopefully ease you into working with push-buttons and LEDs, in addition to writing embedded C code for a microcontroller.
-   It will also ensure your environment is properly set up.

(R1) Using your newfound (or honed skill) of Morse code, what message did Jason in the comic on the first page tap out?

## 1\. Lighting Things Up (safely)

-   Make sure you read the [MPLAB X IDE Set up](https://docs.google.com/document/d/1tdrCcBpLYTH8mV46g0bIkuaSq1z6e9EHJh_3Zq-_rXU/edit?tab=t.0#heading=h.kcb49cfjeajt) document before you start!
-   We need to set up our leds so we can drive them with our microcontroller.
    -   Connect four LEDs to PB1, PB2, PB3, and PB4 on the Atmega328PB Xplained Mini along with a current limiting resistor each.

![](./Lab%202%20-%20Morse%20-%20F25_images/image-002.png)

-   *Figure 1 - LED Resistor Circuit*
    -   To get a better understanding of the pins, check out the [pinout diagram](https://drive.google.com/file/d/1OelIF92Fb1-vnMqhqszeYr3TTAmMgRNs/view?usp=sharing).
    -   It may help to sketch out a circuit showing the connections before you wire them up on the board.
        -   Also, try using the red/blue/green/yellow LEDs to easily tell them apart!
        -   The maximum current that can go through your LED is 20mA. The voltage of your GPIO pins are 5V by default. From this, you should be able to calculate the value of current limiting resistors that you need!
-   Make PB1, PB2, PB3, and PB4 output pins and pull them all high. All of your LEDs should turn on. Refer to the GPIO lecture for guidance.
-   Register manipulation
    -   Recall that manipulating a register is analogous to overwriting bit(s).
    -   For example, if I wanted to set PB1 and PB3 to a high output on PORTB, then I could write PORTB = 0x0A (since 0x0A = 0b00001010), but note that this clears the other bits.
    -   If I wanted to keep the values of the rest of the pins intact, then I could write  
        PORTB |= 0x0A instead.
    -   Note that there is another way to set a register. If I want to set only PB1 high, for example, then I can just do PORTB |= (1 << PORTB1), which will use a macro to set the pin. That way, you don’t have to manually figure out the hex values to set the port to!
    -   If I want to set multiple pins this way, I just need to do  
        PORTB |= (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3) | … and so on.

Hooray! You just wrote your first AVR C code!  

(S1) Take a screenshot of the code that blinks all the LEDs in a loop. I.e. All LED on, then all off, in a loop.

-   You can choose any interval you like.
-   Play around with turning different combinations of LEDs on and off so you get a bit more comfortable manipulating bits.

## 2\. Pushing Buttons (politely)

-   Set up a button to control an LED.
    -   Use one of the buttons in your kit.
    -   Use a 10kΩ resistor to make the input connection to PB0 similar to the circuit below.
    -   The button can be wired on the high side or low side of the resistor.
    -   Make sure your logic is consistent with the behavior that you are expecting - e.g. button press = HIGH, button released = LOW.
    -   ![](./Lab%202%20-%20Morse%20-%20F25_images/image-003.png)

*Figure 2 - Push Button Circuit*

-   Write a C program that toggles an LED when the button is pressed by reading the value of PB0.
    -   The LED should remain on as long as the button is pressed and turn off when the button is released.
-   Remember to set PB0 as an input pin.
-   You can choose whichever pin as an output for the LED to turn on/off when you detect the button press.

(S2) Take a screenshot of the code that toggles an LED when a button is pressed. LED should be on when the button is pressed and off when the button is released

-   With the ability to read from a pin and write to a pin, you can now make your LEDs turn on and off based on how many times you’ve pressed the button!
-   Write code so that each time you press the button, the next LED in sequence turns on and all others are off.
-   Expected sequence of events:
    1.  Initially, only the LED on pin 9 (PB1) should be lit.
    2.  If you press the switch once, then only the LED on pin 10 (PB2) should be lit.
    3.  If you press the switch again, then only the LED on pin 11 (PB3) should be lit.
    4.  If you press the switch once more, then only the LED on pin 12 (PB4) should be lit.
-   This behavior should repeat as you press the button more than 3 times, cycling through the LEDs.
    1.  You may have to press the button and then hold it for a second before releasing it.
    2.  We haven’t gotten to debouncing yet, but if you are familiar with the concept, feel free to implement it. You will not be penalized if there is no debouncing in your code.
-   Tip: You can add a delay using \_delay\_ms() (which takes in a parameter of milliseconds), so that you can see each LED light up between each press. Otherwise, the program may cycle through the different states too quickly for you to see.  
    

(S3) Take a screenshot of your code that turns on the LEDs in a cycle each time a button is pressed.

(S4) Draw your schematic in an ECAD tool. Attach an image to your answer to this question.

This is just building a circuit, not simulating it. You can use Circuitlab, but if you feel more comfortable with another tool like Altium, Eagle, or LTSpice, feel free to use that to build the circuit.

# Part B: Using Interrupts

-   Write an interrupt-based program that will respond by lighting the on-board LED appropriately to input capture events.
    -   Polling can be inefficient since it locks the CPU until an event has occurred. This section should teach you a better way to achieve the same thing.
    -   It should provide the same functionality as the first part of PartA.2, except it will use interrupts rather than polling.
        -   When the button is pressed, the built in LED should be on.
        -   When the button is released, the LED should turn off.
-   You must use PB0 for input capture.
    -   When port PB0 is used for input capture in this fashion, it is referred to as ICP1 in the literature.
    -   Refer to the input capture lecture for guidance.
-   The microcontroller will need to respond to both the low-to-high (button pressed) and high-to-low (button released) transitions to turn on and off the LED at appropriate times.
    -   Set the appropriate edge control bits in the TCCR1B register.
    -   Enable the interrupt in the TIMSK1.
    -   Enable the clock using the TCCR1B register.
    -   Call sei().
-   The interrupt service routine will respond by clearing the input capture flag in TIFR1 by storing a 1 to the ICF1 bit.
-   Reading PORTB, declared as PINB, will determine whether the button was depressed or released, and the routine should set the LED on PORTB accordingly.
-   To write the interrupt routine, you should declare a function with the signature ISR(TIMER1\_CAPT\_vect).

(C1) Write code to accomplish the task described above. Save and upload it as “part\_b.c”.

(R2) Give an advantage and a disadvantage to using interrupts over polling for this task.

# Part C: Dash or Dot

-   Using interrupts, we can figure out whether we have received a dash, a dot, or a space based on the length between presses.
-   For this part:
    -   A dot is defined as the button pressed for approximately 50 ms to 200ms
    -   A dash is defined as the button pressed for approximately 200ms to 400ms
    -   A space is defined as the button not pressed for longer than 400ms
-   When you get to Part E: Putting It Together - LEDs and ASCII, if you find it hard to press the buttons according to these times, you will not be penalized for deciding your own intervals as long as you mention it in the readme. Here is a guideline:
    -   A dot is defined as the button pressed for approximately T/2 to 2T time units
    -   A dash is defined as the button pressed for approximately 2T to 4T time units
    -   A space is defined as the button not pressed for longer than 4T time units.

(R3) For a 16MHz clock, how many “ticks” are in 50ms, 200ms, and 400ms?

-   How do you measure time?
    -   In order to determine the time, it is necessary to examine the TCNT1 register.
    -   However, with the clock running at 16 MHz, and the TCNT1 register being a 16-bit register, the TCNT1 register will overflow approximately every 4ms.
    -   Thus, in order to properly time events longer than 4ms, a timer overflow interrupt (enabled in the TIMSK1 register) or use of the prescaler in TCCR1B is necessary to keep track of the more significant bits of time.
-   Since serial communication will not be covered until later lectures, a short print\_to\_serial library will be used.
    -   Download the files [here](https://github.com/upenn-embedded/avr-printf) and look at the example in main to see how to print to the serial monitor.

## Need some help setting up a serial UART in your project?

1.  Add UART.c and UART.h to your MPLAB X project.  
    Download these files and **place them in the same folder as your existing main.c**

(**Do this in your system finder before adding the existing file in MPLAB**!)

(Make sure the header & source are both **in the exact same folder** with the main.c!)

![](./Lab%202%20-%20Morse%20-%20F25_images/image-004.png)

![](./Lab%202%20-%20Morse%20-%20F25_images/image-005.png)

Use the **Add Existing Item...** command when you right-click a folder.  
Put the **uart.h** file in the Header Files folder.  
Put **uart.c** file in the Source Files folder.  
![](./Lab%202%20-%20Morse%20-%20F25_images/image-006.png)

-   1.  This updates the project file to find all related source files and combine them during compiling/linking time.

1.  Include the library at the top of your main.c file: **#include "uart.h"**
    1.  This assumes you’ve placed the files inside the same folder as your **main.c** source file.
2.  Call **uart\_init();** in your main loop to set things up.
3.  Use it to print! Here's an example snippet of printing text and an integer to the terminal.  
    **static unsigned int ctr = 0;  
    printf("Hello World! Counter: %d\\r\\n", ctr);**
4.  Finally, use a serial terminal to read text transmitted by your MCU. There are many programs to do this - including MPLAB X IDE (sometimes there are issues on Mac OS). Pick what you want for this - you can even go to [https://www.serialterminal.com/](https://www.serialterminal.com/) !  
    ![](./Lab%202%20-%20Morse%20-%20F25_images/image-007.png)

Having issues? You can always use the MPLAB X IDE to make new blank files, then copy and paste the content into them.

-   You can debug the code by having the microcontroller print out the timer register values at certain points in the code execution or on each press and release of the button.
    -   Notice that sometimes there are several events of very short duration - this is due to mechanical bouncing of the contacts on the switch, which you may have already noticed in Part A.
    -   By ignoring these short transition times, the program should be able to handle debouncing robustly.

(R4) Describe how a prescaler allows us to work with a wider range of frequencies on our microcontroller.

# Part D: Hardware Debouncing

-   In the previous part, you have learned about how implementing a small delay can be used to debounce button presses. In this part, you will learn about how using a simple hardware setup, you can achieve a similar result.
    -   When developing embedded systems, considering both hardware and software solutions oftentimes leads to a more optimal design.

(R5) Recall what you have learned before about RC circuits. Using just a resistor and a capacitor, create two separate circuits that function as a high-pass filter and a low-pass filter. Draw sketches of each of these circuits (or build them in Circuit Lab) and submit them.

-   Now, we are going to ask you to build upon the circuit in Figure 2.
-   Create a circuit on your breadboard that follows the schematic in Figure 3.
-   Notice the resistance values for R1 is 1kΩ and R2 is 10kΩ.
-   For the capacitance of C1, start with 1nF for now.
-   Use the probe in Figure 4 to connect nodes and channels correctly.

![](./Lab%202%20-%20Morse%20-%20F25_images/image-008.png)

Figure 3 Circuit schematic for hardware debouncing

(I1) Submit a photo of your circuit setup.

![](./Lab%202%20-%20Morse%20-%20F25_images/image-009.jpeg)![](./Lab%202%20-%20Morse%20-%20F25_images/image-010.jpeg)

Figure 4 Oscilloscope probe and channel connectors

-   For the oscilloscope setting, do the following:
    -   Make sure that you set the trigger more to “Normal” first by pressing the “Mode/Coupling” button on the right of the oscilloscope display.
    -   Make sure you select the appropriate trigger slope by pressing the “Trigger” on the right of the oscilloscope display.
    -   Additionally, make sure that both of your channels are switched on and overlaid together properly by adjusting the appropriate knobs.
-   Now we are going to ask you to take some measurements. Press the button in your circuit and take a photo of your reading on the oscilloscope. You should get a reading that looks like Figure 5.

(I2) Submit a photo/screenshot of your oscilloscope reading.

![](./Lab%202%20-%20Morse%20-%20F25_images/image-011.jpeg)

Figure 5 Sample oscilloscope readings

NOTE: The above sample is a falling edge. Using rising edge may give you a more clear answer for R6, but either edge is acceptable for I2, I3

(I3) Repeat the previous question by changing the capacitance for C1 to 10nF. Submit a photo of your result.

(R6) Which capacitance is more desirable for hardware debouncing? Why? Hint: try zooming in to see the voltage behavior when the button is just pressed.

-   You are free to take apart the setup for this part once you are done with it. We will proceed with this lab with software debouncing.

# Part E: Putting It Together - LEDs and ASCII

-   We can now detect whether we have received a dash or a dot. With this information, it is now time to show some output!
-   We will use LEDs to show whether we received a dash or a dot and then print each character received to the serial port. If you have used different intervals for the dots and dashes, mention it in your readme.
-   Connect two different colored LEDs to the ATmega328PB. Light one up if a dash is received, and light the other one if a dot is received.
    -   The LEDs should only be lit for a brief period of time after the button is released (about 50ms or so).
-   Now, program the microcontroller to decode the dots and dashes into actual alphanumeric characters.
    -   Have the program write out each character’s ASCII value to the serial port so that you can transmit a message successfully.

(C2) Commit the code that achieves this to the repository. Name it “part\_e.c”  

(V1) Submit a link to a demo video. The demo video should show you typing out at least 5 different characters. Repeat this word 2x times to demonstrate repeatability.

Here’s an example video demonstrating what we’re looking for:  
[Lab2MorsePartE\_VideoExample.mp4](https://drive.google.com/file/d/10kzFg87y9H3mJp12XFH3oJ0R4KwNwM_t/view?usp=sharing)

-   Show your microcontroller attached to the computer
-   Show your button and LED wiring
-   Say what you’re going to spell out, then show the button presses, LEDs lighting up, and terminal output on your laptop
-   Repeat your 5x unique character word twice so we can see that your implementation is robust.

A listing for Morse code translations is provided below.

-   Look for a single space (i.e. no button press for >400ms) in a dot/dash sequence to separate each character from one another.

# ![](./Lab%202%20-%20Morse%20-%20F25_images/image-012.png)![](./Lab%202%20-%20Morse%20-%20F25_images/image-013.png)![](./Lab%202%20-%20Morse%20-%20F25_images/image-014.png)

# Part F: The (Dot) Dash to the Finish

Now that you’ve learned to communicate with your microcontroller using a button to input Morse code, let’s reverse it!

(C3) Write a program that takes in a message or word, translates it to Morse code, and then flashes an LED according to the Morse code generated. Name it “part\_f.c”. Include this code in your repository.

Since we haven’t learned about serial communication yet, the message/word can be a hardcoded string in your code that you manually edit, compile, and flash each time you want to change the word. Show at least 12x unique characters.

(V2) Submit a video of this demo.

Here’s an example video demonstrating what we’re looking for:  
[Lab2MorsePartF\_VideoExample.mp4](https://drive.google.com/file/d/10kes0jgar5DkxD4AYpCwR4Ep6eeubSp0/view?usp=drive_link)

-   Show your microcontroller attached to the computer
-   Show your LED wiring
-   Say & show in your code what you’re going to spell out via LED Morse Code
-   Show LEDs lighting up to spell the word you mentioned

# 

# Frequently Asked Questions

Q: I’m plugging in my board, but it’s not lighting up the LEDs or being recognized!  
A: Check if these two jumpers (0R resistors) are populated on your board. Otherwise, contact the teaching team.  
![](./Lab%202%20-%20Morse%20-%20F25_images/image-015.png)

# Submission Requirements

You will submit your assignment through the GitHub Classroom repository and Gradescope submission. See [ESE5190 F25 Assignment Readme](https://docs.google.com/document/d/14to5z6nSutG9GPNAv13D4yna4YaUmD3YhLlwbTLNPXk/edit?tab=t.0) for more information about the submission process.

Submit code written for this lab, separated into a .c file for each part. You’re expected to upload all of your code for every assignment in the semester.

# 

# Grading Rubric

Only the PDF-generated README.md and MPLAB X IDE code solution files in your GitHub Classroom repository will be graded. If your solution(s) used any additional files or dependencies (outside of the upenn-embedded/avr-printf library), you MUST specify this in your README.

If your solution(s) used any additional files or dependencies, you MUST specify this in your README.

| Questions | Points | Number | Total |
| --- | --- | --- | --- |
| R2, R4, R6 | 1 | 3 | 1 x 3 = 3 pt(s) |
| --- | --- | --- | --- |
| R1, R3, R5 | 2 | 3 | 2 x 3 = 6 pt(s) |
| --- | --- | --- | --- |
| I1-I3 | 3 | 3 | 3 x 3 = 9 pt(s) |
| --- | --- | --- | --- |
| S1-S4 | 3 | 4 | 3 x 4 = 12 pt(s) |
| --- | --- | --- | --- |
| C1 | 5 | 1 | 5 x 1 = 5 pt(s) |
| --- | --- | --- | --- |
| C2, C3 | 10 | 2 | 10 x 2 = 20 pt(s) |
| --- | --- | --- | --- |
| V1, V2 | 15 | 2 | 15 x 2 = 30 pt(s) |
| --- | --- | --- | --- |
| Grand Total |  | 85 points |
| --- | --- | --- |