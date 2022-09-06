# Basic Motor control with Rotary Encoder

An experimental circuit where I learn how to use a motor and control it via some rotary encoder.


This project was harder than I wish to admit. The rotary encoder was a nightmare to debounce and I had to rewrite multiple times how I managed the functionality of the display and the motor, even had to completely change my circuit entirely. The original circuit can be found under the branch of `old design`.

The code can be simplified by using a decoder such as the `SN74LS47N`, however, that would defeat the purpose of this assignment (yes, this is a college assignment).

For those wondering this is part of the course Microprocessors (INEL4206) from the University of Puerto Rico. The idea is that students did a simple program in C/C++ where we had some familiarity with either an Arduino or an ESP32 microcontroller.

My assignment is a bit different from the rest, the motor was me playing around with hardware. My classmates used a 4-digit seven-segment display. I unfortunately didn't have one in hand, and didn't wanted to over-complicate myself by connecting two separate 2-digit seven-segment displays in parallel and deal with current drops and whatnot. Adding the decoder should free 3 pins on the microcontroller, you can use a single one and switch between the common display pins (DIG1 and DIG2) to switch between the digits. Below is the datasheet of the display, is important to check if the display pins are on the anode or the cathode, as you will need to change the code a bit for the circuit to work

![](./datasheets/display.webp)

The breadboard circuit can be seen here, go to `datasheets` to obtain the fritzing file.

![](./datasheets/new-circuit-sketch.png)
