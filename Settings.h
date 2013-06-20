/*
 * Settings.h
 *
 *  Created on: Jun 13, 2013
 *      Author: Mike
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#define SETTINGS_VERSION "abc"

struct Settings {
	char version[4];
	float StanceWidth;
	float StanceLength;
	float StepHeight;
	bool SmoothRun;
};


#endif /* SETTINGS_H_ */
