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

void Kinematic::calculateInverse(float x, float y, float z) {
	// Set the x y z cached vars
	this->x = x;
	this->y = y;
	this->z = z;

	// calculate L, it's sued in the others
	this->l = getL();
	this->coxa = toDegrees(getGamma());
	this->femur = toDegrees(getAlpha());
	this->tibia = toDegrees(getBeta());
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
	return (
		acos(z/this->l) +
		acos(
				((TIBIA_LENGTH*TIBIA_LENGTH) - (FEMUR_LENGTH * FEMUR_LENGTH) -(l*l)) /
				(-2 * TIBIA_LENGTH*FEMUR_LENGTH)
		)
	);
}

float Kinematic::getGamma() {
	return atan(x/y);
}

float Kinematic::getBeta() {
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

float Kinematic::toDegrees(float float1) {
	return float1 * 180.0 / 3.14159;
}

void Kinematic::calculateForward(int coxa, int femur, int tibia) {
}

float Kinematic::toRadians(float degrees) {

}
