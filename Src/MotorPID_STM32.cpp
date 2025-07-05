/*
 * MotorPID_STM32.cpp
 *
 *  Created on: Jun 29, 2025
 *      Author: Gabriel
 */

#include "MotorPID_STM32.hpp"

MotorPID_STM32::MotorPID_STM32() :
		MotorPID()
{

}

MotorPID_STM32::MotorPID_STM32(const Configuration& initialConfiguration) :
		MotorPID(initialConfiguration) {

}

MotorPID_STM32::~MotorPID_STM32() {

}

int32_t MotorPID_STM32::init() {
	return 0;
}

int32_t MotorPID_STM32::setSpeed(float newSpeed) {
	return 0;
}
