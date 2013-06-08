/*
 * defs.h
 *
 *  Created on: May 27, 2013
 *      Author: Mike
 */

#ifndef DEFS_H_
#define DEFS_H_

#define BUILDSTRING "Alphy v0.0.1"

// Servo array IDs
#define FRONT_LEFT_COXA 0
#define FRONT_LEFT_FEMUR 1
#define FRONT_LEFT_TIBIA 2

#define FRONT_RIGHT_COXA 3
#define FRONT_RIGHT_FEMUR 4
#define FRONT_RIGHT_TIBIA 5

#define BACK_LEFT_COXA 6
#define BACK_LEFT_FEMUR 7
#define BACK_LEFT_TIBIA 8

#define BACK_RIGHT_COXA 9 // Up is inward
#define BACK_RIGHT_FEMUR 10 // no, something with certain pins, 24 doesn't work
#define BACK_RIGHT_TIBIA 11

// Servo pin mappings
#define FRONT_LEFT_COXA_PIN 45
#define FRONT_LEFT_FEMUR_PIN 44
#define FRONT_LEFT_TIBIA_PIN 43

#define FRONT_RIGHT_COXA_PIN 41
#define FRONT_RIGHT_FEMUR_PIN 40
#define FRONT_RIGHT_TIBIA_PIN 39

#define BACK_LEFT_COXA_PIN 29
#define BACK_LEFT_FEMUR_PIN 28
#define BACK_LEFT_TIBIA_PIN 27

#define BACK_RIGHT_COXA_PIN 25
#define BACK_RIGHT_FEMUR_PIN 24
#define BACK_RIGHT_TIBIA_PIN 23

// If the servo value needs to be inverted because it was placed backwards
// Servo pin mappings
#define FRONT_LEFT_COXA_INVERT true
#define FRONT_LEFT_FEMUR_INVERT true
#define FRONT_LEFT_TIBIA_INVERT false

#define FRONT_RIGHT_COXA_INVERT false
#define FRONT_RIGHT_FEMUR_INVERT false
#define FRONT_RIGHT_TIBIA_INVERT true

#define BACK_LEFT_COXA_INVERT false
#define BACK_LEFT_FEMUR_INVERT false
#define BACK_LEFT_TIBIA_INVERT true

#define BACK_RIGHT_COXA_INVERT true
#define BACK_RIGHT_FEMUR_INVERT true
#define BACK_RIGHT_TIBIA_INVERT false


#define SERVO_MIN 30
#define SERVO_MAX 150

#define DEBUG_SERIAL1
#define DEBUG_SERIAL
#define SERIAL_SPEED 115200
#define SERIALCOMMAND_DEBUG

#define TIBIA_LENGTH 2.0
#define FEMUR_LENGTH 2.0
#define COXA_LENGTH 2.0

#define WEIGHT_FOR_LOOPSPEED = 0.3

#define DEFAULT_RUN_SPEED 1000


#endif /* DEFS_H_ */
