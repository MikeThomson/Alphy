/*
 * ExecutionPlan.cpp
 *
 *  Created on: Jun 5, 2013
 *      Author: Mike
 */

#include "ExecutionPlan.h"

ExecutionPlan::ExecutionPlan(int steps, void (*cb)(int, int)) {
	// TODO Auto-generated constructor stub
	stepCount = steps;
	//plan = new int[1][12];
	callback = cb;
	delay = 0;
	currentStep = 0;
	stepsAdded = 0;
	timePassed = 0;
}

void ExecutionPlan::registerServoCallback(void (*cb)(int, int)) {
	callback = cb;
}

bool ExecutionPlan::run(long timeElapsed) {
	if (currentStep == stepCount)
		return true;
	timePassed -= timeElapsed;
	if (timePassed <= 0 && !smooth) { // simple step
		timePassed = delay - timePassed; // The lefotver. should check for going past 0 again
		for (int i = 0; i < 12; i++) {
			callback(i, plan[currentStep][i]);
		}
		currentStep++;
		if (currentStep == stepCount)
			return true;
	} else if (smooth) { // we should update with fractional steps
		if (timePassed <= 0) { // we're in the next step
			currentStep++;
			timePassed = delay - timePassed;
		}
		if (currentStep == stepCount - 1) { // just do a simple update because there's nowhere to go next
			for (int i = 0; i < 12; i++) {
				callback(i, plan[currentStep][i]);
			}
			currentStep++; // Kinda a hacky way to do this, but it works for now
		} else {
			float fractionPassed = (delay - timePassed) / (float) delay; // probably should clamp or handle 0 < x < 1
			for (int i = 0; i < 12; i++) {
				int angle = plan[currentStep][i] + (int)((plan[currentStep + 1][i] - plan[currentStep][i]) * fractionPassed);
				callback(i, angle);
			}
		}

	}
	return false;
}

bool ExecutionPlan::run() {
	if (currentStep == stepCount)
		return true;
	for (int i = 0; i < 12; i++)
		callback(i, plan[currentStep][i]);
	currentStep++;
	if (currentStep == stepCount)
		return true;
	return false;
}

bool ExecutionPlan::isDone() {
	return currentStep == stepCount;
}

bool ExecutionPlan::addStep(int step[12]) {
	if (stepsAdded == stepCount)
		return false;
	for (int i = 0; i < 12; i++)
		plan[stepsAdded][i] = step[i];
	stepsAdded++;
	return true;
}

int ExecutionPlan::getCurrentStep() {
	return currentStep;
}

int ExecutionPlan::getStepCount() {
	return stepCount;
}

void ExecutionPlan::setDelay(long wait) {
	delay = wait;
	timePassed = wait;
}

void ExecutionPlan::reset() {
	currentStep = 0;
	timePassed = delay;
}

void ExecutionPlan::setSmooth(bool onOff) {
	smooth = onOff;
}
