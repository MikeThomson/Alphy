/*
 * debug.h
 *
 *  Created on: Jun 11, 2013
 *      Author: Mike
 */

#ifndef DEBUG_H_
#define DEBUG_H_
#include <Arduino.h>
#include "defs.h"
//#include "floatToString.h"

void debug(String);
void debug(int);
void debug(long);
void debug(float);



#endif /* DEBUG_H_ */
