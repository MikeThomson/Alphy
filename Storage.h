/*
 * Storage.h
 *
 *  Created on: Jun 8, 2013
 *      Author: Mike
 */

#ifndef STORAGE_H_
#define STORAGE_H_
#include <Arduino.h>
#include <EEPROM.h>
#include "ExecutionPlan.h"
#include "defs.h"

#define STORAGE_EEPROM_SIZE 4096
/*
 * Storage is mapped as follows:
 * 0-11: Initial servo angles
 * 12-239: Reserved for future program data
 * 240-4095: Execution plan storage
 *
 * At each plan address, An ExecutionPlan is stored as follows:
 * 0: Number of steps in the plan
 * 1-12: first step
 * 2-24: second step, etc
 *
 * Plan 'id' address can be calculated with:
 * 240 + (25 * id)
 */

class Storage {
public:
	Storage();
	static int* getInitialPosition();
	static int saveInitialPosition(int* servos);

	static ExecutionPlan* getPlan(int id);
	static void savePlan(ExecutionPlan* plan, int id);

	static void dumpEeprom(void (*cb)(byte*), int length);
	static void rawWriteEeprom(byte* , int address,int length);

	/*
	static Settings loadSettings() ;
	static void saveSettings(Settings settings) ;
*/

private:
	static bool used[16];
	static bool stepIsValid(int* step);

	static int getPlanAddress(int id);
	static int getStepAddress(int id, int step);

	class ExecutionPlanCallback {
	public:
		ExecutionPlanCallback(int id,int numSteps, int (*addressFunc)(int, int));
		void writeCallback(int servo, int position);

	private:
		int planId;
		int stepCount;
		int currentStep;
		int callsInCurrentStep;
		int (*getAddress)(int, int) ;


	};
};

#endif /* STORAGE_H_ */
