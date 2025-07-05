/*
 * MotorPID_STM32.hpp
 *
 *  Created on: Jun 29, 2025
 *      Author: Gabriel
 */

#ifndef SHARED_ROBOFRAMEWORK_STM32_SRC_MOTORPID_STM32_HPP_
#define SHARED_ROBOFRAMEWORK_STM32_SRC_MOTORPID_STM32_HPP_

#include "MotorPID.hpp"

class MotorPID_STM32 : public MotorPID {
public:
	MotorPID_STM32();
	MotorPID_STM32(const Configuration& initialConfiguration);
	virtual ~MotorPID_STM32();
	int32_t init();
	int32_t setSpeed(float newSpeed);
};

#endif /* SHARED_ROBOFRAMEWORK_STM32_SRC_MOTORPID_STM32_HPP_ */
