// Do not remove the include below
#include "Alphy.h"

SerialCommand serialCommand;

int currentServo = 0;

Servo servos[12];
int servoAngles[12];

ExecutionPlan *plan;

long averageLoopSpeed;
long lastTime = 0;

bool inPlanMode = false;
bool running = false;
bool continuous = false;

Kinematic kinematic;

//The setup function is called once at startup of the sketch
void setup() {
// Add your initialization code here
#ifdef DEBUG_SERIAL1
	Serial1.begin(SERIAL_SPEED);
#endif
#ifdef DEBUG_SERIAL
	Serial.begin(SERIAL_SPEED);
#endif
	initCommands();
	initServos();
	loadInitialPosition();
	averageLoopSpeed = 0;
	lastTime = millis();
}

void continuousOn() {
	continuous = true;
}

void continuousOff() {
	continuous = false;
}

long weightedAverageFilter(long reading, long currentValue, float weight) {
	return reading + (long) (weight * (currentValue - reading));
}

// The loop function is called in an endless loop
void loop() {
	serialCommand.readSerial();
	long currentTime = millis();
	long elapsedTime = currentTime - lastTime;
	lastTime = currentTime;
	if(running) {
		if(plan->run(elapsedTime)) {
			if(continuous) {
				plan->reset();
			}
			else running = false;
		}
	}
	averageLoopSpeed = weightedAverageFilter(elapsedTime, averageLoopSpeed, 0.3);
}

void initCommands() {
	serialCommand.addCommand("PING", pong);
	serialCommand.addCommand("SELECT", selectServo);
	serialCommand.addCommand("SET", setServo);
	serialCommand.addCommand("TIME", time);
	serialCommand.addCommand("FREE", freeMem);
	serialCommand.addCommand("STATS", stats);
	serialCommand.addCommand("PLAN", planMode);
	serialCommand.addCommand("ENDPLAN", endPlanMode);
	serialCommand.addCommand("STEP", addPlanStep);
	serialCommand.addCommand("RUN", run);
	serialCommand.addCommand("RESET", resetPlan);
	serialCommand.addCommand("SAVESTATE", saveInitialPosition);
	serialCommand.addCommand("KIN", moveLeg);
	serialCommand.addCommand("SWEEP", sweepLeg);
	serialCommand.addCommand("CONT", continuousOn);
	serialCommand.addCommand("NOCONT", continuousOff);
	serialCommand.addCommand("LOADCREEP", loadCreep);
	serialCommand.setDefaultHandler(unrecognized);
}

void initServos() {
	servos[FRONT_RIGHT_COXA].attach(FRONT_RIGHT_COXA_PIN);
	servos[FRONT_LEFT_COXA].attach(FRONT_LEFT_COXA_PIN);
	servos[BACK_RIGHT_COXA].attach(BACK_RIGHT_COXA_PIN);
	servos[BACK_LEFT_COXA].attach(BACK_LEFT_COXA_PIN);

	servos[FRONT_RIGHT_FEMUR].attach(FRONT_RIGHT_FEMUR_PIN);
	servos[FRONT_LEFT_FEMUR].attach(FRONT_LEFT_FEMUR_PIN);
	servos[BACK_RIGHT_FEMUR].attach(BACK_RIGHT_FEMUR_PIN);
	servos[BACK_LEFT_FEMUR].attach(BACK_LEFT_FEMUR_PIN);

	servos[FRONT_RIGHT_TIBIA].attach(FRONT_RIGHT_TIBIA_PIN);
	servos[FRONT_LEFT_TIBIA].attach(FRONT_LEFT_TIBIA_PIN);
	servos[BACK_RIGHT_TIBIA].attach(BACK_RIGHT_TIBIA_PIN);
	servos[BACK_LEFT_TIBIA].attach(BACK_LEFT_TIBIA_PIN);

}

int constrainAngle(int angle) {
	return constrain(angle, SERVO_MIN, SERVO_MAX);
}

void writeServo(int servo, int angleSet) {
	bool invert = false;
	switch (servo) {
	case FRONT_RIGHT_COXA:
		invert = FRONT_RIGHT_COXA_INVERT;
		break;
	case FRONT_RIGHT_FEMUR:
		invert = FRONT_RIGHT_FEMUR_INVERT;
		break;
	case FRONT_RIGHT_TIBIA:
		invert = FRONT_RIGHT_TIBIA_INVERT;
		break;

	case FRONT_LEFT_COXA:
		invert = FRONT_LEFT_COXA_INVERT;
		break;
	case FRONT_LEFT_FEMUR:
		invert = FRONT_LEFT_FEMUR_INVERT;
		break;
	case FRONT_LEFT_TIBIA:
		invert = FRONT_LEFT_TIBIA_INVERT;
		break;

	case BACK_RIGHT_COXA:
		invert = BACK_RIGHT_COXA_INVERT;
		break;
	case BACK_RIGHT_FEMUR:
		invert = BACK_RIGHT_FEMUR_INVERT;
		break;
	case BACK_RIGHT_TIBIA:
		invert = BACK_RIGHT_TIBIA_INVERT;
		break;

	case BACK_LEFT_COXA:
		invert = BACK_LEFT_COXA_INVERT;
		break;
	case BACK_LEFT_FEMUR:
		invert = BACK_LEFT_FEMUR_INVERT;
		break;
	case BACK_LEFT_TIBIA:
		invert = BACK_LEFT_TIBIA_INVERT;
		break;
	}

	int finalAngle = 90;
	if (invert) {
		finalAngle = constrainAngle(180 - angleSet);
		servoAngles[servo] = 180 - finalAngle; // need to reinvert to save the constrained angle
	} else {
		finalAngle = constrainAngle(angleSet);
		servoAngles[servo] = finalAngle;
	}
	debug("SERVO ");
	debug(servo);
	debug(" ");
	debug(finalAngle);
	debug("\r\n");
	servos[servo].write(finalAngle);

}

void pong() {
	debug("PONG\r\n");
}

int readServo(int servo) {
	return servoAngles[servo];
}

void selectServo() {
	char *arg;
	arg = serialCommand.next();
	if (arg != NULL) {
		currentServo = atoi(arg);
		debug("SERVO ");
		debug(currentServo);
		debug(" SELECTED\r\n");
	} else {
		debug("ERR: BAD ARGS\r\n");
	}
}

void moveLeg() {
	char *arg;
	float x,y,z;

	arg = serialCommand.next();
	if (arg != NULL) {
		currentServo = atoi(arg);
	} else {
		debug("BAD ARGS\r\n");
	}

	arg = serialCommand.next();
	if (arg != NULL) {
		x = ::atof(arg);
	} else {
		debug("BAD ARGS\r\n");
	}

	arg = serialCommand.next();
	if (arg != NULL) {
		y = ::atof(arg);
	} else {
		debug("BAD ARGS\r\n");
	}

	arg = serialCommand.next();
	if (arg != NULL) {
		z = ::atof(arg);
	} else {
		debug("BAD ARGS\r\n");
	}

	moveLegToPoint(currentServo, x, y, z);
}

void unrecognized(const char *command) {
	debug("ERR: UNKNOWN COMMAND\r\n");
}

void moveLegToPoint(int legCoxa, float x, float y, float z) {
	kinematic.calculateInverse(z, y, z);
	debug("Coxa: ");
	debug(kinematic.getCoxaAngle());
	debug("\r\n");
	debug("Femur: ");
	debug(kinematic.getFemurAngle());
	debug("\r\n");
	debug("Tibia: ");
	debug(kinematic.getTibiaAngle());
	debug("\r\n");
}

void setServo() {
	char *arg;
	int servo;
	int angle;
	// get the servo
	arg = serialCommand.next();
	if (arg != NULL) {
		servo = atoi(arg);
	} else {
		debug("ERR: NO SERVO SPECIFIED\r\n");
		return;
	}

	// Get the angle
	arg = serialCommand.next();
	if (arg != NULL) {
		angle = atoi(arg);
	} else {
		debug("ERR: NO ANGLE SPECIFIED\r\n");
		return;
	}

	if(servo < 0 || servo > 11) {
		debug("ERR: INVALID SERVO\r\n");
		return;
	}
	if(angle < SERVO_MIN || angle > SERVO_MAX) {
		debug("ERR: INVALID ANGLE\r\n");
		return;
	}
	writeServo(servo, angle);
}

void freeMem() {
	debug("FREE MEMORY: ");
	debug(freeMemory());
	debug("\r\n");
}

void time() {
	debug("TIME: ");
	debug((long) millis());
	debug("\r\n");
}

void stats() {
	version();
	time();
	freeMem();
	loopSpeed();
}

void version() {
	debug(BUILDSTRING);
	debug("\r\n");
}

void loopSpeed() {
	debug("LOOP SPEED: ");
	debug(averageLoopSpeed);
	debug("\r\n");
}

void planMode() {
	inPlanMode = true;
	delete plan;
	int runSpeed = DEFAULT_RUN_SPEED;
	int stepCount = 0;
	char *arg;

	arg = serialCommand.next();
	if(arg != NULL) {
		stepCount = atoi(arg);
	} else {
		debug("ERR: NO COUNT GIVEN\r\n");
	}
	arg = serialCommand.next();
	if (arg != NULL) {
		runSpeed = atol(arg);
	}

	plan = new ExecutionPlan(stepCount, writeServo);
	plan->setDelay(runSpeed);
}

void endPlanMode() {
	inPlanMode = false;
}

void addPlanStep() {
	char *arg;
	int step[12];
	bool success = true;
	for (int i = 0; i < 12; i++) {
		arg = serialCommand.next();
		if (arg != NULL) {
			step[i] = atoi(arg);
		} else {
			debug("ERR: ");
			debug(i);
			debug("IS TOO FEW ANGLES\r\n");
			success = false;
			break;
		}
	}

	if(success) {
		if(!plan->addStep(step)) {
			debug("ERR: TOO MANY STEPS\r\n");
		}
		else {
			debug("STEP ADDED\r\n");
		}
	}

}

void run() {
	running = true;
}

/*
 * Initial position is the first 12 bytes of EEPROM. If any of the bytes
 * are outside SERVO_MIN and SERVO_MAX, the position is invalid and 90 is
 * written to every servo
 */

void saveInitialPosition() {
	debug("SAVING INITIAL POSITION\r\n");
	for(int i=0;i<12;i++) {
		EEPROM.write(i,(byte)readServo(i));
	}
	debug("INITIAL POSITION SAVED\r\n");
}

void loadInitialPosition() {
	debug("LOADING INITIAL POSITION\r\n");
	int temp[12];
	bool success = true;
	for (int i = 0; i < 12; i++) {
		temp[i] = EEPROM.read(i);
		if(temp[i] < SERVO_MIN || temp[i] > SERVO_MAX) {
			debug("INVALID STATE IN EEPROM\r\n");
			success = false;
			break;
		}
	}
	if(success) {
		for(int i =0; i<12;i++) {
			writeServo(i, temp[i]);
		}
		debug("INITIAL POSITION LOADED\r\n");
	} else {
		for (int i = 0; i < 12; i++) {
			writeServo(i, 90);
		}
	}

}

void dumpEeprom() {

}

void getEeprom() {

}

void resetPlan() {
	plan->reset();
}

void sweepLeg() {
	float x = 1;
	float y = 3;
	float z = 1.5;

	for(;x>-1.0;x-=0.1) {
		kinematic.calculateInverse(x,y,z);
		writeServo(0, kinematic.getCoxaAngle());
		writeServo(1, kinematic.getFemurAngle());
		writeServo(2, kinematic.getTibiaAngle());
		delay(150);
	}
}

void loadCreep() {
	long speed;
	float rideHeight;
	float stepHeight = 1.0;
	float stanceWidth = 10.5;
	float stanceLength = 6.0;
	float stepLength;

	char *arg;

	arg = serialCommand.next();
	if (arg != NULL) {
		speed = atol(arg);
	} else {
		debug("ERR: NO SPEED GIVEN\r\n");
	}
	arg = serialCommand.next();
	if (arg != NULL) {
		rideHeight = ::atof(arg);
	} else {
		debug("ERR: NO HEIGHT GIVEN\r\n");
	}
	arg = serialCommand.next();
	if (arg != NULL) {
		stepLength = ::atof(arg);
	} else {
		debug("ERR: NO STEP LENGTH GIVEN\r\n");
	}

	CreepGait* gait = new CreepGait(&kinematic, rideHeight, stepLength, stanceWidth, stanceLength, stepHeight);
	plan = gait->getPlan();
	plan->registerServoCallback(writeServo);
	plan->setDelay(speed);
	plan->reset();
}
