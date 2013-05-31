/*
 * Kinematic.h
 *
 *  Created on: May 30, 2013
 *      Author: Mike
 */

#ifndef KINEMATIC_H_
#define KINEMATIC_H_
#include "defs.h"
#include <math.h>

class Kinematic {
public:
	Kinematic();
	void calculate(float x, float y, float z);
	int getCoxaAngle();
	int getFemurAngle();
	int getTibiAngle();
private:
	int coxa;
	int femur;
	int tibia;
	float l;
	float x, y, z;
	int getAlpha();
	int getGamma();
	int getBeta();
	float getL();
};

#endif /* KINEMATIC_H_ */
