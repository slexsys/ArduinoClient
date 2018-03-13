#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include "ADServer.h"

#define USE_SERIAL			Serial
#define SERIAL_SPEED		115200

class Logger {
	public:
		static void log(String text);
		static void logx(String text);

	private:
		static bool inited;
		static void init();
};

bool Logger::inited = false;

void Logger::log(String text) {
	logx(text + "\n");
}

void Logger::logx(String text) {
	init();
	USE_SERIAL.printf("%s", text.c_str());
	//USE_SERIAL.printf("# count: %d", count);
}

void Logger::init() {
	if (!inited) {
		inited = true;
		USE_SERIAL.begin(SERIAL_SPEED);
	}
}

#endif

