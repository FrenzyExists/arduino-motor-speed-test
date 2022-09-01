#include <Arduino.h>

#define CLK A2
#define DT  A1
#define SW  A0
#define M1PIN1 11
#define M1PIN2 12
#define CONTROL 13

#define _A 2
#define _B 3
#define _C 4
#define _D 5
#define _E 6
#define _F 7
#define _G 8

#define SEGMENTGND1 9
#define SEGMENTGND2 10

int currentStateCLK;
int previousStateCLK; 
int motorSpeed = 0;

int counter = 0;
const int threshold = 100;
int state = true;
int index;


/**
 * Scale is 2.58 between the two seven digit display (0 - 99) and the motor speed (0 - 255)
 */

void setup() {
	pinMode(SW, INPUT);
  	digitalWrite(SW, HIGH);
  	pinMode(CLK, INPUT);
  	pinMode(DT, INPUT);

	pinMode(M1PIN1, OUTPUT);
	pinMode(M1PIN2, OUTPUT);
	pinMode(CONTROL, OUTPUT);

	pinMode(_A, OUTPUT);
	pinMode(_B, OUTPUT);
	pinMode(_C, OUTPUT);
	pinMode(_D, OUTPUT);
	pinMode(_E, OUTPUT);
	pinMode(_F, OUTPUT);
	pinMode(_G, OUTPUT);

	pinMode(SEGMENTGND2, OUTPUT);
	pinMode(SEGMENTGND2, OUTPUT);
}

void rotateLeft() {
	motorSpeed = motorSpeed < -255 ? -255 : motorSpeed - 5;
}

void rotateRight() {
	motorSpeed = motorSpeed > 255 ? 255 : motorSpeed + 5;
}

void loop() {
  // Read the current state of inputCLK
	currentStateCLK = digitalRead(CLK);

	if (currentStateCLK != previousStateCLK){ 
		// If the inputDT state is different than the inputCLK
		// state then the encoder is rotating counterclockwise
		if (digitalRead(DT) != currentStateCLK) {
			rotateRight();
		} else {
			rotateLeft();
		}
	}
	
	if (motorSpeed < -10) {
		analogWrite(CONTROL, abs(motorSpeed) );
		digitalWrite(M1PIN1, HIGH);
		digitalWrite(M1PIN2, LOW);
	} else if (motorSpeed >= 10) {
		analogWrite(CONTROL, abs(motorSpeed));
		digitalWrite(M1PIN1, LOW);
  		digitalWrite(M1PIN2, HIGH);
	}

	/**
	 * Ok, so uh, This following section is a dirty trick I did where I take advantage of the
	 * arduinos' clock and two NPN transistors transistors as switches controlled by the arduino. 
	 * 
	 * first digit is obtained by
	 * floor(round(motorSpeed / 2.58) / 10)
	 * 
	 * second is obtained by
	 * round(motorSpeed / 2.58) % 10
	 */

	if (counter == threshold) {
		state = !state;
		if (state) {
			index = floor(round(abs(motorSpeed) / 2.55) / 10);
			analogWrite(SEGMENTGND2, 100);
			analogWrite(SEGMENTGND1, 0);
			
		} else {
			index = floor(round(abs(motorSpeed) / 2.55) % 10);
			analogWrite(SEGMENTGND2, 0);
			analogWrite(SEGMENTGND1, 100);
		}
		counter = 0;
	}

	if (index == 0) {
		digitalWrite(_A, HIGH);
		digitalWrite(_B, HIGH);
		digitalWrite(_C, HIGH);
		digitalWrite(_D, HIGH);
		digitalWrite(_E, HIGH);
		digitalWrite(_F, HIGH);
		digitalWrite(_G, LOW);
	} else if (index == 1) {
		digitalWrite(_A, LOW);
		digitalWrite(_B, HIGH);
		digitalWrite(_C, HIGH);
		digitalWrite(_D, LOW);
		digitalWrite(_E, LOW);
		digitalWrite(_F, LOW);
		digitalWrite(_G, LOW);
	} else if (index == 2) {
		digitalWrite(_A, HIGH);
		digitalWrite(_B, HIGH);
		digitalWrite(_C, LOW);
		digitalWrite(_D, HIGH);
		digitalWrite(_E, HIGH);	
		digitalWrite(_F, LOW);
		digitalWrite(_G, HIGH);
	} else if (index == 3) {
		digitalWrite(_A, HIGH);
		digitalWrite(_B, HIGH);
		digitalWrite(_C, HIGH);
		digitalWrite(_D, HIGH);
		digitalWrite(_E, LOW);	
		digitalWrite(_F, LOW);
		digitalWrite(_G, HIGH);
	} else if (index == 4) {
		digitalWrite(_A, LOW);
		digitalWrite(_B, HIGH);
		digitalWrite(_C, HIGH);
		digitalWrite(_D, LOW);
		digitalWrite(_E, LOW);
		digitalWrite(_F, HIGH);
		digitalWrite(_G, HIGH);
	} else if (index == 5) {
		digitalWrite(_A, 180);
		digitalWrite(_B, LOW);
		digitalWrite(_C, HIGH);
		digitalWrite(_D, HIGH);
		digitalWrite(_E, LOW);
		digitalWrite(_F, HIGH);
		digitalWrite(_G, HIGH);
	} else if (index == 6) {
		digitalWrite(_A, 180);
		digitalWrite(_B, LOW);
		digitalWrite(_C, HIGH);
		digitalWrite(_D, HIGH);
		digitalWrite(_E, HIGH);
		digitalWrite(_F, HIGH);
		digitalWrite(_G, HIGH);
	} else if (index == 7) {
		digitalWrite(_A, HIGH);
		digitalWrite(_B, HIGH);
		digitalWrite(_C, HIGH);
		digitalWrite(_D, LOW);
		digitalWrite(_E, LOW);
		digitalWrite(_F, LOW);
		digitalWrite(_G, LOW);
	} else if (index == 8) {
		digitalWrite(_A, HIGH);
		digitalWrite(_B, HIGH);
		digitalWrite(_C, HIGH);
		digitalWrite(_D, HIGH);
		digitalWrite(_E, HIGH);
		digitalWrite(_F, HIGH);
		digitalWrite(_G, HIGH);
	} else if (index == 9) {
		digitalWrite(_A, HIGH);
		digitalWrite(_B, HIGH);
		digitalWrite(_C, HIGH);
		digitalWrite(_D, LOW);
		digitalWrite(_E, LOW);
		digitalWrite(_F, HIGH);
		digitalWrite(_G, HIGH);
	}
	
	counter++;
	previousStateCLK = currentStateCLK; // update state
}