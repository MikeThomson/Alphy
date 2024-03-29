/*
 * ExecutionPlan.h
 *
 *  Created on: Jun 5, 2013
 *      Author: Mike
 */

#ifndef EXECUTIONPLAN_H_
#define EXECUTIONPLAN_H_

#include <Arduino.h>

class ExecutionPlan {
public:
	ExecutionPlan(int stepCount, void (*cb)(int, int));
	void registerServoCallback(void (*cb)(int, int));
	bool run(long timeElapsed); // returns true if the plan has executed it's final step
	bool run();
	bool isDone();
	bool addStep(int step[12]);
	int getCurrentStep();
	int getStepCount();
	void setDelay(long wait);
	void reset();
	void setSmooth(bool onOff = true);


private:
	int plan[20][12];
	int currentStep;
	int stepCount;
	long timePassed;
	long delay;
	int stepsAdded;
	void (*callback)(int, int);
	bool smooth;


};

#endif /* EXECUTIONPLAN_H_ */
