#ifndef DIGITALPORTS_H
#define DIGITALPORTS_H

#include <Arduino.h>

class DigitalPins {
	public:
		DigitalPins(int* pins);
		~DigitalPins();
		
		void pinOn(int index);
		void pinOff(int index);

	private:
		int* _pins;

		void initPins();
		void initPin(int index);
};

DigitalPins::DigitalPins(int* pins) : _pins(pins) {
	initPins();
}

void DigitalPins::pinOn(int index) {
	digitalWrite(_pins[index], LOW);
}

void DigitalPins::pinOff(int index) {
	digitalWrite(_pins[index], HIGH);
}

void DigitalPins::initPins() {
	for (int i = 0; i < sizeof(_pins); ++i) {
		initPin(i);
	}
}

void DigitalPins::initPin(int index) {
	pinMode(_pins[index], OUTPUT);
  digitalWrite(_pins[index], HIGH);
}

#endif

