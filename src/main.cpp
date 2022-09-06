#include <Arduino.h>

#define CLK A1
#define DT  A0
// #define M1PIN1 11
// #define M1PIN2 12
// #define CONTROL 13

#define _A 2
#define _B 3
#define _C 4
#define _D 5
#define _E 6
#define _F 7
#define _G 8

#define SEGMENTGND1 9
#define SEGMENTGND2 10
int commons[] = {
  SEGMENTGND1, SEGMENTGND2
};
int commonLength = sizeof(commons)/sizeof(commons[0]);


uint8_t previousStateCLK; 
int numberDisplay = 0;
int rotaryCounter = 0;
int secondCounter = 0;

int counter = 0;
const int threshold = 100;
int state = true;
int index;
int segments[] = {
	_A, _B, _C, _D, _E, _F, _G
};
int segmentLength = sizeof(segments)/sizeof(segments[0]);

int displayDigits[] = {
  0b00111111, /* 0 */
  0b00000110, /* 1 */
  0b01011011, /* 2 */
  0b01001111, /* 3 */
  0b01100110, /* 4 */
  0b01101101, /* 5 */
  0b01111101, /* 6 */
  0b00000111, /* 7 */
  0b01111111, /* 8 */
  0b01100111  /* 9 */
};

#define A_BIT 1
#define B_BIT 2
#define C_BIT 4
#define D_BIT 8
#define E_BIT 16
#define F_BIT 32
#define G_BIT 64
int digitPosition[] = {A_BIT, B_BIT, C_BIT, D_BIT, E_BIT, F_BIT, G_BIT};

int digitCounter = 0;
int delayCounter = 16;
bool stateCommon = false;

/**
 * Rotary Encoder functions
*/

void rotateLeft() {
	numberDisplay = numberDisplay < -99 ? -99 : numberDisplay - 1;
}

void rotateRight() {
	numberDisplay = numberDisplay >= 99 ? 99 : numberDisplay + 1;
}

void rotaryEncoderCtrl() {
	// Read the current state of inputCLK
	uint8_t currentStateCLK = 0;
	currentStateCLK = digitalRead(DT) << 1 | digitalRead(CLK);

	if (currentStateCLK == previousStateCLK) { return; }

	if (rotaryCounter == 16) {
		secondCounter++;
		if (secondCounter == 4) {
			if (bitRead(currentStateCLK, 0) == bitRead(previousStateCLK, 1)) { 
					rotateRight();
			} else {
					rotateLeft();
			}
			Serial.println(numberDisplay);
			secondCounter = 0;
		}
		previousStateCLK = currentStateCLK; // update state
	}
	rotaryCounter = rotaryCounter == 16 ? 0 : rotaryCounter+1;
}



void setMotorSpeed(int speed, int delta) {
	
}

void resetDisplay() {
	for (int i = 0; i < segmentLength ; i++) {
		digitalWrite(segments[i], HIGH);
	}
	for (int i = 0 ; i < commonLength ; i++) {
		digitalWrite(commons[i], LOW);
	}
}

void displayOneDigit(int n,int pos) {
  for (int i = 0; i < segmentLength; i++) {
    if (displayDigits[n] & digitPosition[i]) 
      digitalWrite(segments[i], LOW);
    }
    digitalWrite(commons[pos], HIGH);
}

void initDisplays() {
	// Turn on segment 
	pinMode(SEGMENTGND1, OUTPUT);
	pinMode(SEGMENTGND2, OUTPUT);

	for (int i = 0 ; i < segmentLength ; i++ ) {
		pinMode(segments[i], OUTPUT);
	}
	resetDisplay();
}

void displayAllDisplay() {
	if (digitCounter == delayCounter) {
		state = !state;
		if (state) {
			displayOneDigit(abs(floor(numberDisplay % 10)), 0);
		} else {
			displayOneDigit(abs(floor(numberDisplay / 10)), 1);
		}
		resetDisplay();
		digitCounter = 0;
	}
	digitCounter++;

}

void setup() {
  	pinMode(CLK, INPUT_PULLUP);
  	pinMode(DT, INPUT_PULLUP);

	initDisplays();
	resetDisplay();

	previousStateCLK = digitalRead(DT) << 1 | digitalRead(CLK);
	attachInterrupt(0, rotaryEncoderCtrl, CHANGE);
  	attachInterrupt(1, rotaryEncoderCtrl, CHANGE);
	Serial.begin(9600);
}


void loop() {
	rotaryEncoderCtrl();
	displayAllDisplay();
}