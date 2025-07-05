/*
 * Encoder_STM32.cpp
 *
 *  Created on: Jul 5, 2025
 *      Author: Gabriel
 */

#include "Encoder_STM32.hpp"

Encoder_STM32::Encoder_STM32(TIM_HandleTypeDef* htim) : htim(htim) {

}

Encoder_STM32::~Encoder_STM32() {

}

int32_t Encoder_STM32::init() {
	if(HAL_TIM_Base_Start(htim) != HAL_OK){
		return -1;
	}
	if(HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL) != HAL_OK){
		return -1;
	}
	initialized = true;
	return 0;
}

int32_t Encoder_STM32::getPosition(int32_t *position) {
	if(!initialized){
		return -1;
	}
	*position = __HAL_TIM_GET_COUNTER(htim);
	return 0;
}
