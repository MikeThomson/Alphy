/*
 * Kinematic.cpp
 *
 *  Created on: May 30, 2013
 *      Author: Mike
 */

#include "Kinematic.h"

	const float coxaLength = 2.0;
	const float femurLength = 2.0;
	const float tibiaLength = 2.0;

Kinematic::Kinematic() :
	x(0),
	y(0),
	z(0),
	coxa(90),
	femur(90),
	tibia(90),
	l(0)
{
	// TODO Auto-generated constructor stub
}

void Kinematic::calculateInverse(float x, float y, float z) {
	// Set the x y z cached vars
	this->x = x;
	this->y = y;
	this->z = z;

	// calculate L, it's sued in the others
	this->l = getL();
	this->coxa = (int)toDegrees(getGamma()) + 90;
	this->femur = (int)toDegrees(getAlpha());
	this->tibia = (int)toDegrees(getBeta());
}

int Kinematic::getCoxaAngle() {
	return coxa;
}

int Kinematic::getFemurAngle() {
	return femur;
}

int Kinematic::getTibiaAngle() {
	return tibia;
}

float Kinematic::getAlpha() {
	// Commented out to copy kinematics found on http://engineer-this.com/QuadrupedVer2.txt
	/*
	return (
		acos(z/this->l) +
		acos(
				((TIBIA_LENGTH*TIBIA_LENGTH) - (FEMUR_LENGTH * FEMUR_LENGTH) -(l*l)) /
				(-2 * TIBIA_LENGTH*FEMUR_LENGTH)
		)
	);
	*/
	float tempX = x;
	float g = atan(tempX / y);
	tempX /= cos(g);
	float L2 = sqrt(pow(z, 2) + pow(y - coxaLength, 2));
	float alp1 = atan((y - coxaLength) / z);
	float alp2 = acos(
			(pow(tibiaLength, 2) - pow(femurLength, 2) - pow(L2, 2)) / (-2 * femurLength * L2));
	float alp = alp1 + alp2;
	return alp;
}

float Kinematic::getGamma() {
	return atan(x/y); // Probably convert to atan2,
}

float Kinematic::getBeta() {
	/*
	return (
			acos(
					((l*l)-(TIBIA_LENGTH*TIBIA_LENGTH) - (FEMUR_LENGTH * FEMUR_LENGTH)) /
					(-2 * TIBIA_LENGTH*FEMUR_LENGTH)
			)
	);
	*/

	float tempX = x;
	float g = atan(tempX/y);
	tempX/=cos(g);
	float bet =acos((pow(z,2)+pow(y-coxaLength,2)-pow(tibiaLength,2)-pow(femurLength,2))/(-2*femurLength*tibiaLength));
	return bet;
}

float Kinematic::getL() {
	return sqrt(z*z + y*y);
}

float Kinematic::toDegrees(float float1) {
	return float1 * 57.3;
}

void Kinematic::calculateForward(int coxa, int femur, int tibia) {
}

float Kinematic::toRadians(float degrees) {
	return degrees / 57.3;
}
