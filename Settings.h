/*
 * Settings.h
 *
 *  Created on: Jun 13, 2013
 *      Author: Mike
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#define SETTINGS_VERSION_MAJOR 1
#define SETTINGS_VERSION_MINOR 1

struct Settings {

	Settings() :  version1(SETTINGS_VERSION_MAJOR), version2(SETTINGS_VERSION_MINOR),StanceWidth(8.0), StanceLength(9.0), StepHeight(2.0), SmoothRun(false) {}

	char version1;
	char version2;
	float StanceWidth;
	float StanceLength;
	float StepHeight;
	bool SmoothRun;
};


#endif /* SETTINGS_H_ */
