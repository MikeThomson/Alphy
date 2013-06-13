/*
 * CreepGait.h
 *
 *  Created on: Jun 9, 2013
 *      Author: Mike
 */

#ifndef CREEPGAIT_H_
#define CREEPGAIT_H_

#include <Arduino.h>
#include <ExecutionPlan.h>
#include "Kinematic.h"
#include "defs.h"
#include "debug.h"

class CreepGait {
public:
	CreepGait(Kinematic* k,float h, float step, float sideSpread, float forwardSpread, float stepHeight);
	ExecutionPlan* getPlan();
	struct LegPlanArray ;
	struct StepArray ;
	struct FullPlanArray ;

private:
	float height;
	float stepLength;
	float sideLegSpread;
	float forwardLegSpread;
	float stepHeight;

	Kinematic* kin;

	CreepGait::LegPlanArray generateFrontLeg();
	CreepGait::LegPlanArray generateBackLeg();
	CreepGait::LegPlanArray generateLegFromCoords( float centerX, float forwardX, float backwardX, float downZ, float upZ, float yOffset) ;
	CreepGait::FullPlanArray interleaveLegs(LegPlanArray front, LegPlanArray back);
	CreepGait::StepArray getStep(float x, float y, float z);
	int wrapIndex(int index);

};

#endif /* CREEPGAIT_H_ */
