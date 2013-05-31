// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef Alphy_H_
#define Alphy_H_
#include "defs.h"
#include "Arduino.h"
#include <Servo.h>
#include <SerialCommand.h>
//add your includes for the project Alphy here


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project Alphy here

//void serialEvent1();
void processCommand(String command);
void simpleLower();
void simpleRaise();
int constrainAngle(int angle);
void initServos();
void writeServo(int servo, int angle);
int readServo(int servo);
void pong();
void selectServo();
void initCommands();
void unrecognized(const char *command);
void raiseServo();
void lowerServo();

void lowerFemurs();
void raiseFemurs();
void raiseTibias();
void lowerTibias();
void insideCoxas();
void outerCoxas();

//Do not add code below this line
#endif /* Alphy_H_ */
