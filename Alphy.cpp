// Do not remove the include below
#include "Alphy.h"


String commandBuffer;
boolean commandComplete;
SerialCommand serialCommand;

int currentServo = 0;

Servo servos[12];
int servoAngles[12];
//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	commandBuffer = "";
	commandBuffer.reserve(256);

	commandComplete = false;

	Serial1.begin(115200);
	initCommands();
	initServos();
}

// The loop function is called in an endless loop
void loop()
{
	serialCommand.readSerial();
	//Add your repeated code here
	if(commandComplete) {
		processCommand(commandBuffer);
		commandBuffer = "";
		commandComplete = false;
	}

}

void initCommands() {
	serialCommand.addCommand("PING", pong);
	serialCommand.addCommand("SELECT", selectServo);
	serialCommand.addCommand("u", raiseServo);
	serialCommand.addCommand("d", lowerServo);
	serialCommand.addCommand("uf", raiseFemurs);
	serialCommand.addCommand("df", lowerFemurs);
	serialCommand.addCommand("ut", raiseTibias);
	serialCommand.addCommand("dt", lowerTibias);
	serialCommand.addCommand("ic", insideCoxas);
	serialCommand.addCommand("oc", outerCoxas);
	serialCommand.setDefaultHandler(unrecognized);
}
/*
void serialEvent1() {
	while (Serial1.available()) {
		// get the new byte:
		char inChar = (char) Serial1.read();
		// Echo it back for debugging
		Serial1.print(inChar);
		// add it to the inputString:

		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:
		if (inChar == '\r') {
			commandComplete = true;
		}
		else {
			commandBuffer += inChar;
		}
	}
}
*/
void processCommand(String command) {
	Serial1.println("Got command: " + command);
	//int servo = 0;
	//int angle = 90;

	/*
	String servo = command.substring(0,1);
	String angle = command.substring(2);

	int iServo = servo.toInt();
	int iAngle = angle.toInt();
*/

	if(command == "d") {
		simpleLower();
	} else if(command == "u") {
		simpleRaise();
	}

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

	for(int i=0;i<12;i++) {
		writeServo(i, 90);
	}
}

void simpleRaise() {
	servos[FRONT_RIGHT_FEMUR].write(constrainAngle(servos[FRONT_RIGHT_FEMUR].read()+10));
	servos[FRONT_LEFT_FEMUR].write(constrainAngle(servos[FRONT_LEFT_FEMUR].read()+10));
	servos[BACK_RIGHT_FEMUR].write(constrainAngle(servos[BACK_RIGHT_FEMUR].read()+10));
	servos[BACK_LEFT_FEMUR].write(constrainAngle(servos[BACK_LEFT_FEMUR].read()+10));

	servos[FRONT_RIGHT_TIBIA].write(constrainAngle(servos[FRONT_RIGHT_TIBIA].read()+10));
	servos[FRONT_LEFT_TIBIA].write(constrainAngle(servos[FRONT_LEFT_TIBIA].read()+10));
	servos[BACK_RIGHT_TIBIA].write(constrainAngle(servos[BACK_RIGHT_TIBIA].read()+10));
	servos[BACK_LEFT_TIBIA].write(constrainAngle(servos[BACK_LEFT_TIBIA].read()+10));
}

void simpleLower() {
	servos[FRONT_RIGHT_FEMUR].write(constrainAngle(servos[FRONT_RIGHT_FEMUR].read()-10));
	servos[FRONT_LEFT_FEMUR].write(constrainAngle(servos[FRONT_LEFT_FEMUR].read()-10));
	servos[BACK_RIGHT_FEMUR].write(constrainAngle(servos[BACK_RIGHT_FEMUR].read()-10));
	servos[BACK_LEFT_FEMUR].write(constrainAngle(servos[BACK_LEFT_FEMUR].read()-10));

	servos[FRONT_RIGHT_TIBIA].write(constrainAngle(servos[FRONT_RIGHT_TIBIA].read()-10));
	servos[FRONT_LEFT_TIBIA].write(constrainAngle(servos[FRONT_LEFT_TIBIA].read()-10));
	servos[BACK_RIGHT_TIBIA].write(constrainAngle(servos[BACK_RIGHT_TIBIA].read()-10));
	servos[BACK_LEFT_TIBIA].write(constrainAngle(servos[BACK_LEFT_TIBIA].read()-10));
}

int constrainAngle(int angle) {
	return constrain(angle, SERVO_MIN, SERVO_MAX);
}

void writeServo(int servo, int angleSet) {
	bool invert = false;
	switch(servo) {
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
	if(invert) {
		finalAngle = constrainAngle(180 - angleSet);
		servoAngles[servo] = 180-finalAngle; // need to reinvert to save the constrained angle
	} else {
		finalAngle = constrainAngle(angleSet);
		servoAngles[servo] = finalAngle;
	}
	Serial1.print("Servo ");
	Serial1.print(servo);
	Serial1.print(" set to ");
	Serial1.println(finalAngle);
	servos[servo].write(finalAngle);

}

void raiseServo() {
	writeServo(currentServo, servoAngles[currentServo] + 10);
}

void lowerServo() {
	writeServo(currentServo, servoAngles[currentServo] - 10);
}

void lowerFemurs() {
	writeServo(FRONT_RIGHT_FEMUR,45);
	writeServo(FRONT_LEFT_FEMUR,45);
	writeServo(BACK_RIGHT_FEMUR,45);
	writeServo(BACK_LEFT_FEMUR,45);
}

void lowerTibias() {
	writeServo(FRONT_RIGHT_TIBIA,45);
	writeServo(FRONT_LEFT_TIBIA,45);
	writeServo(BACK_RIGHT_TIBIA,45);
	writeServo(BACK_LEFT_TIBIA,45);
}

void raiseFemurs() {
	writeServo(FRONT_RIGHT_FEMUR,135);
	writeServo(FRONT_LEFT_FEMUR,135);
	writeServo(BACK_RIGHT_FEMUR,135);
	writeServo(BACK_LEFT_FEMUR,135);
}

void raiseTibias() {
	writeServo(FRONT_RIGHT_TIBIA,135);
	writeServo(FRONT_LEFT_TIBIA,135);
	writeServo(BACK_RIGHT_TIBIA,135);
	writeServo(BACK_LEFT_TIBIA,135);
}

void insideCoxas() {
	writeServo(FRONT_RIGHT_COXA,45);
	writeServo(FRONT_LEFT_COXA,45);
	writeServo(BACK_RIGHT_COXA,45);
	writeServo(BACK_LEFT_COXA,45);
}

void outerCoxas() {
	writeServo(FRONT_RIGHT_COXA,135);
	writeServo(FRONT_LEFT_COXA,135);
	writeServo(BACK_RIGHT_COXA,135);
	writeServo(BACK_LEFT_COXA,135);
}

void pong() {
	Serial1.println("PONG");
}

int readServo(int servo) {
	return servoAngles[servo];
}

void selectServo() {
	char *arg;
	arg = serialCommand.next();
	if(arg != NULL) {
		currentServo = atoi(arg);
		Serial1.print("SERVO ");
		Serial1.print(currentServo);
		Serial1.println(" SELECTED");
	} else {
		Serial1.println("BAD ARGS");
	}
}

void unrecognized(const char *command) {
	Serial1.println("UNKNOWN");
}
