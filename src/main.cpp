#include <Arduino.h>

#define CLK A0
#define DT  A1
#define SW  A2
#define M1PIN1 6
#define M1PIN2 7
#define CONTROL 9

#define A 2
#define B 3
#define C 4
#define D 5

#define SEGMENTGND1 10
#define SEGMENTGND2 11

int currentStateCLK;
int previousStateCLK; 
int motorSpeed = 100;

int counter = 0;
const threshold = 100;
int state = true;
int index;

const int numberBit[10][4] =  {
    { LOW, LOW, LOW, LOW    },
	{ LOW, LOW, LOW, HIGH   },
	{ LOW, LOW, HIGH, LOW   },
	{ LOW, LOW, HIGH, HIGH  },
	{ LOW, HIGH, LOW, LOW   },
	{ LOW, HIGH, LOW, HIGH  },
	{ LOW, HIGH, HIGH, LOW  },
	{ LOW, HIGH, HIGH, HIGH },
	{ HIGH, LOW, LOW, LOW   },
	{ HIGH, LOW, LOW, HIGH  }
};


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

	pinMode(A, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(C, OUTPUT);
	pinMode(D, OUTPUT);
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
	
	if (motorSpeed  < -5 || motorSpeed >= 5) {	// Motor is off
		analogWrite(CONTROL, 0);
		digitalWrite(M1PIN1, LOW);
  		digitalWrite(M1PIN2, LOW);
	} else if (motorSpeed < -6) {
		analogWrite(CONTROL, abs(motorSpeed) );
		digitalWrite(M1PIN1, HIGH);
  		digitalWrite(M1PIN2, LOW);
	} else if (motorSpeed >= 6) {
		analogWrite(CONTROL, motorSpeed);
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
			index = floor(round(motorSpeed / 2.58) / 10);
			digitalWrite(SEGMENTGND2, LOW);
			digitalWrite(SEGMENTGND1, HIGH);
		} else {
			index = floor(round(motorSpeed / 2.58) % 10);
			digitalWrite(SEGMENTGND1, LOW);
			digitalWrite(SEGMENTGND2, HIGH);
		}
	}

	digitalWrite(A, numberBit[index][0]);
	digitalWrite(B, numberBit[index][1]);
	digitalWrite(C, numberBit[index][2]);
	digitalWrite(D, numberBit[index][3]);
	

	counter++;
	previousStateCLK = currentStateCLK; // update state
}