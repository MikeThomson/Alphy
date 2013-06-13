/*
 * CreepGait.cpp
 *
 *  Created on: Jun 9, 2013
 *      Author: Mike
 */

#include "CreepGait.h"

const float BODY_WIDTH = 2.75;
const float BODY_LENGTH = 4.125;

struct CreepGait::LegPlanArray {
		int steps[16][3];
};

struct CreepGait::FullPlanArray {
		int steps[16][12];
};

struct CreepGait::StepArray{
		int angles[3];
};

void debugXYZ(int step, float x, float y, float z) {
	debug("STEP XZY: ");
	debug(step);
	debug(" ");
	debug(x);
	debug(" ");
	debug(y);
	debug(" ");
	debug(z);
	debug("\r\n");
}

CreepGait::LegPlanArray CreepGait::generateFrontLeg() {
	// calculate the enpoints of the leg
	float yOffset = (sideLegSpread - BODY_WIDTH) / 2.0;

	float centerX = (forwardLegSpread - BODY_LENGTH) / 2.0;
	float forwardX = centerX + (stepLength / 2);
	float backwardX = centerX - (stepLength / 2);

	float downZ = height;
	float upZ = height - 1.0;

	return generateLegFromCoords(centerX, forwardX, backwardX, downZ, upZ, yOffset);
}

CreepGait::LegPlanArray CreepGait::generateBackLeg() {
	// calculate the enpoints of the leg
	float yOffset = (sideLegSpread - BODY_WIDTH) / 2.0;

	float centerX = - (forwardLegSpread - BODY_LENGTH) / 2.0; // made negative to offset the legs away from the body, not inward
	float forwardX = centerX + (stepLength / 2);
	float backwardX = centerX - (stepLength / 2);

	float downZ = height;
	float upZ = height - 1.0;

	return generateLegFromCoords(centerX, forwardX, backwardX, downZ, upZ, yOffset);
}

CreepGait::LegPlanArray CreepGait::generateLegFromCoords(float centerX,
		float forwardX, float backwardX, float downZ, float upZ,
		float yOffset) {

	LegPlanArray arr = { };
	float fastXOffset = stepLength / 4;
	float slowXOffset = stepLength / 12;

	// rising beat
	// first position is down and all the way back
	StepArray s = getStep(backwardX, yOffset, downZ);

	debugXYZ(0, backwardX, yOffset, downZ);
	for (int j = 0; j < 3; j++)
		arr.steps[0][j] = s.angles[j];

	s = getStep(backwardX + fastXOffset, yOffset, upZ);
	debugXYZ(1, backwardX + fastXOffset, yOffset, upZ);
	for (int j = 0; j < 3; j++)
		arr.steps[1][j] = s.angles[j];

	s = getStep(backwardX + (2 * fastXOffset), yOffset, upZ);
	debugXYZ(2, backwardX + (2 * fastXOffset), yOffset, upZ);
	for (int j = 0; j < 3; j++)
		arr.steps[2][j] = s.angles[j];

	s = getStep(backwardX + forwardX, yOffset, upZ);
	debugXYZ(3, backwardX + forwardX, yOffset, upZ);
	for (int j = 0; j < 3; j++)
		arr.steps[3][j] = s.angles[j];

	// for the rest of the gait this leg just slowly moves backward
	for (int i = 4; i < 16; i++) {
		s = getStep(forwardX - ((i - 3) * slowXOffset), yOffset, downZ);
		debugXYZ(i, forwardX - ((i - 3) * slowXOffset), yOffset, downZ);
		for (int j = 0; j < 3; j++)
			arr.steps[i][j] = s.angles[j];
	}

	return arr;

}

CreepGait::FullPlanArray CreepGait::interleaveLegs(LegPlanArray front, LegPlanArray back) {
	//int steps[16][12];
	FullPlanArray plan;

	// The beat that the leg should start rising
	int rightRearStart = 0;
	int rightFrontStart = 4;
	int leftRearStart = 8;
	int leftFrontStart = 12;

	for(int i=0;i<16;i++) {
		plan.steps[wrapIndex(i+rightRearStart)][BACK_RIGHT_COXA] = back.steps[i][0];
		plan.steps[wrapIndex(i+rightRearStart)][BACK_RIGHT_FEMUR] = back.steps[i][1];
		plan.steps[wrapIndex(i+rightRearStart)][BACK_RIGHT_TIBIA] = back.steps[i][2];

		plan.steps[wrapIndex(i+rightFrontStart)][FRONT_RIGHT_COXA] = front.steps[i][0];
		plan.steps[wrapIndex(i+rightFrontStart)][FRONT_RIGHT_FEMUR] = front.steps[i][1];
		plan.steps[wrapIndex(i+rightFrontStart)][FRONT_RIGHT_TIBIA] = front.steps[i][2];

		plan.steps[wrapIndex(i+leftRearStart)][BACK_LEFT_COXA] = back.steps[i][0];
		plan.steps[wrapIndex(i+leftRearStart)][BACK_LEFT_FEMUR] = back.steps[i][1];
		plan.steps[wrapIndex(i+leftRearStart)][BACK_LEFT_TIBIA] = back.steps[i][2];

		plan.steps[wrapIndex(i+leftFrontStart)][FRONT_LEFT_COXA] = front.steps[i][0];
		plan.steps[wrapIndex(i+leftFrontStart)][FRONT_LEFT_FEMUR] = front.steps[i][1];
		plan.steps[wrapIndex(i+leftFrontStart)][FRONT_LEFT_TIBIA] = front.steps[i][2];
	}

	return plan;
}

struct CreepGait::StepArray CreepGait::getStep(float x, float y, float z) {
	StepArray s;
	//int* step = new int[3];
	kin->calculateInverse(x,y,z);
	s.angles[0] = kin->getCoxaAngle();
	s.angles[1] = kin->getFemurAngle();
	s.angles[2] = kin->getTibiaAngle();
	return s;
}

CreepGait::CreepGait(Kinematic *k, float h, float step, float sideSpread,
		float forwardSpread, float sHeight) {
	kin = k;
	height = h;
	stepLength = step;
	sideLegSpread = sideSpread;
	forwardLegSpread = forwardSpread;
	stepHeight = sHeight;
}

ExecutionPlan* CreepGait::getPlan() {
	FullPlanArray a = interleaveLegs(generateFrontLeg(), generateBackLeg());

	ExecutionPlan* plan = new ExecutionPlan(16, NULL);
	for(int i=0;i<16;i++) {
		plan->addStep(a.steps[i]);
	}
	return plan;
}

int CreepGait::wrapIndex(int index) {
	return index % 16;
}
