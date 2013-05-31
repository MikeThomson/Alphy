/*
 * Kinematic.cpp
 *
 *  Created on: May 30, 2013
 *      Author: Mike
 */

#include "Kinematic.h"

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

void Kinematic::calculate(float x, float y, float z) {
	// Set the x y z cached vars
	this->x = x;
	this->y = y;
	this->z = z;

	// calculate L, it's sued in the others
	this->l = getL();
	this->coxa = getGamma();
	this->femur = getAlpha();
	this->tibia = getBeta();
}

int Kinematic::getCoxaAngle() {
	return coxa;
}

int Kinematic::getFemurAngle() {
	return femur;
}

int Kinematic::getTibiAngle() {
	return tibia;
}

int Kinematic::getAlpha() {
	return (
		acos(z/this->l) +
		acos(
				((TIBIA_LENGTH*TIBIA_LENGTH) - (FEMUR_LENGTH * FEMUR_LENGTH) -(l*l)) /
				(-2 * TIBIA_LENGTH*FEMUR_LENGTH)
		)
	);
}

int Kinematic::getGamma() {
	return atan(x/y);
}

int Kinematic::getBeta() {
	return (
			acos(
					((l*l)-(TIBIA_LENGTH*TIBIA_LENGTH) - (FEMUR_LENGTH * FEMUR_LENGTH)) /
					(-2 * TIBIA_LENGTH*FEMUR_LENGTH)
			)
	);
}

float Kinematic::getL() {
	return sqrt(z*z + y*y);
}
