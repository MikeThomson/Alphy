/*
 * debug.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: Mike
 */
#include "debug.h"
#include "floatToString.h"

void debug(String string) {
#ifdef DEBUG_SERIAL
	Serial.print(string);
#endif
#ifdef DEBUG_SERIAL1
	Serial1.print(string);
#endif
}

void debug(int string) {
	debug(String(string));
}

void debug(long string) {
	debug(String((long) string));
}

void debug(float string) {
	char buffer[25];
	debug(floatToString(buffer, string, 5));
}

