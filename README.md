Motor test run (sort of)

An experimental circuit where I learn how to use a motor and control it via some rotary encoder.

Didn't wanted to complicate myself so I used a 7-digit display (2 digits)

I only had a single encoder/decoder chip, and normally you would need 2 for 2 digits, so intead I used transistors whose gates are triggered by a signal from the arduino in order to give the illusion that both displays are on at the same time, a lil trick I learned from the blinky project

Update: my decoder/encoder chip was dead, the code in here is WITHOUT the chip. This means I had to use almost all PWM pins available. Note that because of I'm using a Leonardo whose pins are limited (good moment to thing about getting an atMega), the pins 13 and 12 which are used cannot output a perfect 5V, and therefore, you may notice a difference in speed on the motor between one direction and the other
