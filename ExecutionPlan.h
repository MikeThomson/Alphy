/*
 * ExecutionPlan.h
 *
 *  Created on: Jun 5, 2013
 *      Author: Mike
 */

#ifndef EXECUTIONPLAN_H_
#define EXECUTIONPLAN_H_

class ExecutionPlan {
public:
	ExecutionPlan(int stepCount, void (*cb)(int, int));
	void registerServoCallback(void (*cb)(int, int));
	bool run(long timeElapsed); // returns true if the plan has executed it's final step
	bool isDone();
	bool addStep(int step[12]);
	int getCurrentStep();
	int getStepCount();
	void setDelay(long wait);
	void reset();

private:
	int plan[10][12];
	int currentStep;
	int stepCount;
	long timePassed;
	long delay;
	int stepsAdded;
	void (*callback)(int, int);

};

#endif /* EXECUTIONPLAN_H_ */
