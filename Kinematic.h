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
	void calculateInverse(float x, float y, float z);
	void calculateForward(int coxa, int femur, int tibia);
	int getCoxaAngle();
	int getFemurAngle();
	int getTibiaAngle();
private:
	int coxa;
	int femur;
	int tibia;
	float l;
	float x, y, z;
	float getAlpha();
	float getGamma();
	float getBeta();
	float getL();
	float toDegrees(float);
	float toRadians(float degrees);


};

#endif /* KINEMATIC_H_ */
