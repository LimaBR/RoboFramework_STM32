/*
 * Encoder_STM32.hpp
 *
 *  Created on: Jul 5, 2025
 *      Author: Gabriel
 */

#ifndef SHARED_ROBOFRAMEWORK_STM32_INC_ENCODER_STM32_HPP_
#define SHARED_ROBOFRAMEWORK_STM32_INC_ENCODER_STM32_HPP_

#include "main.h"
#include "Encoder.hpp"

class Encoder_STM32 : public Encoder {
public:
	Encoder_STM32(TIM_HandleTypeDef* htim);
	virtual ~Encoder_STM32();
	int32_t init();
	int32_t getPosition(int32_t* position);
private:
	TIM_HandleTypeDef* htim;
	bool initialized = false;
};

#endif /* SHARED_ROBOFRAMEWORK_STM32_INC_ENCODER_STM32_HPP_ */
