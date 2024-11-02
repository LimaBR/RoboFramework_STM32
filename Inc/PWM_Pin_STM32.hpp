/*
 * PWM_Pin_STM32.hpp
 *
 *  Created on: Jul 30, 2024
 *      Author: Gabriel
 */

#ifndef SRC_COMPONENTS_PWM_PIN_STM32_HPP_
#define SRC_COMPONENTS_PWM_PIN_STM32_HPP_

#include "main.h"
#include "PWM_Pin.hpp"

class PWM_Pin_STM32 : public PWM_Pin {
public:
	PWM_Pin_STM32(TIM_HandleTypeDef* htim, uint32_t channel);
	virtual ~PWM_Pin_STM32();
	int32_t init();
	int32_t setDutyCycle(float dutyCycle);
	int32_t getFrequency(float* frequency);
	int32_t reset();
private:
	TIM_HandleTypeDef* htim;
	uint32_t channel;
};

#endif /* SRC_COMPONENTS_PWM_PIN_STM32_HPP_ */
