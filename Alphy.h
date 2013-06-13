// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef Alphy_H_
#define Alphy_H_
#include "defs.h"
#include "Arduino.h"
#include "debug.h"
#include <EEPROM.h>
#include <Servo.h>
#include <SerialCommand.h>
#include <MemoryFree.h>
#include "Kinematic.h"
#include "ExecutionPlan.h"
#include "CreepGait.h"
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

int constrainAngle(int angle);
void initServos();
void writeServo(int servo, int angle);
int readServo(int servo);
void pong();
void selectServo();
void initCommands();
void unrecognized(const char *command);
void freeMem();

void moveLeg();
void moveLegToPoint(int, float, float, float);

void setServo();
void time();
void stats();
void version();
void loopSpeed();
void run();

void planMode();
void endPlanMode();
void addPlanStep();
void viewPlan();
void resetPlan();

void saveInitialPosition() ;
void loadInitialPosition() ;

void savePLan() ;
void loadPlan() ;

void sweepLeg();
void loadCreep();

void continuousOn();
void continuousOff();

long weightedAverageFilter(long reading, long currentValue, float weight);

//Do not add code below this line
#endif /* Alphy_H_ */
