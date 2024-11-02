/*
 * PWM_Pin_STM32.cpp
 *
 *  Created on: Jul 30, 2024
 *      Author: Gabriel
 */

#include "PWM_Pin_STM32.hpp"

PWM_Pin_STM32::PWM_Pin_STM32(TIM_HandleTypeDef* htim, uint32_t channel) : htim(htim), channel(channel) {

}

PWM_Pin_STM32::~PWM_Pin_STM32() {

}

int32_t PWM_Pin_STM32::init() {
	if(HAL_TIM_PWM_Start(htim, channel) == HAL_OK){
		return 0;
	}else{
		return -1;
	}
}

int32_t PWM_Pin_STM32::setDutyCycle(float dutyCycle) {
	if(dutyCycle<0 || dutyCycle>1){
		// dutyCycle out of range
		return -1;
	}
	uint32_t arr = __HAL_TIM_GET_AUTORELOAD(htim);
	uint32_t ccr = dutyCycle*arr;
	__HAL_TIM_SET_COMPARE(htim, channel, ccr);
	return 0;
}

int32_t PWM_Pin_STM32::getFrequency(float* frequency){
	// FIXME: May work only on STM32F4
	float timClkFreq;
	if((uint32_t*)htim->Instance >= (uint32_t*)APB1PERIPH_BASE && (uint32_t*)htim->Instance < (uint32_t*)APB2PERIPH_BASE){
		// Timer on APB1
		timClkFreq = HAL_RCC_GetPCLK1Freq()*2;
	}else if((uint32_t*)htim->Instance > (uint32_t*)APB2PERIPH_BASE){
		// Timer on APB2
		timClkFreq = HAL_RCC_GetPCLK2Freq()*2;
	}else{
		// Unknown bus
		return -1;
	}
	if(frequency){
		*frequency = timClkFreq/((htim->Instance->ARR+1) * (htim->Instance->PSC+1));
	}
	return 0;
}

int32_t PWM_Pin_STM32::reset(){
	// TODO: test if this implementation works to force reload
	__HAL_TIM_SET_COUNTER(htim, 0);
	return 0;
}
