/*
 * Storage.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: Mike
 */

#include "Storage.h"

const int CONFIG_START = 13;

Storage::Storage() {
	// TODO Auto-generated constructor stub

}

int* Storage::getInitialPosition() {
}

int Storage::saveInitialPosition(int* servos) {
}

ExecutionPlan* Storage::getPlan(int id) {
	int address = getPlanAddress(id);
	int stepCount = EEPROM.read(address);
	if(stepCount < 0 || stepCount > 20) return NULL;
	ExecutionPlan* plan = new ExecutionPlan(stepCount, NULL);

	for(int i=0;i<stepCount;i++) {
		int stepAddress = getStepAddress(id, i);
		int step[12];
		for(int j = 0;j<12;j++) {
			step[j] = EEPROM.read(stepAddress+j);
		}
		if(!stepIsValid(step)) {
			delete plan;
			return NULL;
		}
		if(!plan->addStep(step)) {
			delete plan;
			return NULL;
		}
	}
	return plan;

}

void Storage::savePlan(ExecutionPlan* plan, int id) {
	ExecutionPlanCallback* epc = new ExecutionPlanCallback(id, plan->getStepCount(), getStepAddress);
	plan->reset();
	int count = plan->getStepCount();
	for(int i=0;i<count;i++) {
		plan->run();
	}
	delete epc;
}

void Storage::dumpEeprom(void (*cb)(byte*), int length) {
	int cycles = STORAGE_EEPROM_SIZE / length;
	for(int i=0;i<cycles;i++) {
		byte bytes[length];
		for(int j=0;j<length;j++) {
			bytes[j] = EEPROM.read((i*length) + j);
		}
		cb(bytes);
	}
}

void Storage::rawWriteEeprom(byte* bytes, int address,int length) {
	for(int i=0;i<length;length++) {
		EEPROM.write(address+i, bytes[i]);
	}
}

bool Storage::stepIsValid(int* step) {
	for(int i=0;i<12;i++)
		if(step[i] < SERVO_MIN || step[i] > SERVO_MAX)
			return false;

	return true;
}

int Storage::getPlanAddress(int id) {
	return 240 + (25 * id);
}

Settings* Storage::loadSettings() {
	Settings* settings;
	// To make sure there are settings, and they are YOURS!
	// If nothing is found it will use the default settings.
	if (EEPROM.read(CONFIG_START + 0) == SETTINGS_VERSION[0]
			&& EEPROM.read(CONFIG_START + 1) == SETTINGS_VERSION[1]
			&& EEPROM.read(CONFIG_START + 2) == SETTINGS_VERSION[2])
		for (unsigned int t = 0; t < sizeof(settings); t++)
			*((char*) &settings + t) = EEPROM.read(CONFIG_START + t);
	return settings;
}

void Storage::saveSettings(Settings settings) {
	for (unsigned int t=0; t<sizeof(settings); t++)
		EEPROM.write(CONFIG_START + t, *((char*)&settings + t));
}

int Storage::getStepAddress(int id, int step) {
	return getPlanAddress(id) + (step * 12) + 1;
}

Storage::ExecutionPlanCallback::ExecutionPlanCallback(int id, int numSteps,
		int (*addressFunc)(int, int)) {
	getAddress = addressFunc;
	stepCount = numSteps;
	currentStep = 0;
	callsInCurrentStep = 0;
	planId = id;
}

void Storage::ExecutionPlanCallback::writeCallback(int servo, int position) {
	if(currentStep == stepCount) return;// Dont overwrite memory that's not ours on accident
	EEPROM.write(getAddress(planId, currentStep) + servo, (byte) position);
	callsInCurrentStep++;
	if(callsInCurrentStep == 12) {
		currentStep++;
		callsInCurrentStep = 0;
	}
}
