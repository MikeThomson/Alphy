// Do not remove the include below
#include "Alphy.h"

SerialCommand serialCommand;

int currentServo = 0;

Servo servos[12];
int servoAngles[12];

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
}

// The loop function is called in an endless loop
void loop() {
	serialCommand.readSerial();

}

void initCommands() {
	serialCommand.addCommand("PING", pong);
	serialCommand.addCommand("SELECT", selectServo);
	serialCommand.addCommand("SET", setServo);
	serialCommand.addCommand("TIME", time);
	serialCommand.addCommand("FREE", freeMem);
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

	for (int i = 0; i < 12; i++) {
		writeServo(i, 90);
	}
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
	debug(" [");
	debug(finalAngle);
	debug("]\r\n");
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
		debug("BAD ARGS\r\n");
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
	debug("UNKNOWN COMMAND\r\n");
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

void setServo() {
	char *arg;
	int servo;
	int angle;
	// get the servo
	arg = serialCommand.next();
	if (arg != NULL) {
		servo = atoi(arg);
	} else {
		debug("NO SERVO SPECIFIED\r\n");
		return;
	}

	// Get the angle
	arg = serialCommand.next();
	if (arg != NULL) {
		angle = atoi(arg);
	} else {
		debug("NO ANGLE SPECIFIED\r\n");
		return;
	}

	if(servo < 0 || servo > 11) {
		debug("INVALID SERVO\r\n");
		return;
	}
	if(angle < SERVO_MIN || angle > SERVO_MAX) {
		debug("INVALID ANGLE\r\n");
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
	debug("TIME ");
	debug((long) millis());
	debug("\r\n");
}
