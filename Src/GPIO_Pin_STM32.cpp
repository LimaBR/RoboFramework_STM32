/*
 * GPIO_Pin_STM32.cpp
 *
 *  Created on: Feb 10, 2024
 *      Author: Gabriel
 */

#include "GPIO_Pin_STM32.hpp"
#include <cstdio>

// Free functions

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	GPIO_Pin_STM32::extiDispatch(GPIO_Pin);
}

// Constructors and destructors

/**	\brief Constructor
 * 	\param[in] gpio_port STM32 HAL GPIO port mask
 * 	\param[in] gpio_pin STM32 HAL GPIO pin number
 */
GPIO_Pin_STM32::GPIO_Pin_STM32(GPIO_TypeDef *gpio_port, uint16_t gpio_pin) : gpio_port(gpio_port), gpio_pin(gpio_pin) {
	pinNumber = gpioPinToPinNumber(gpio_pin);
}

GPIO_Pin_STM32::~GPIO_Pin_STM32() {

}

// Public methods

/**
 * @brief Initializes GPIO Pin
 *
 * @return Error code
 * @retval 0 Success
 */
int32_t GPIO_Pin_STM32::init() {
	return 0;
}

/**	\brief Read GPIO pin state
 *  \details Reads STM32 digital pin state using HAL Library
 * 	\param[out] state State of the pin read
 * 	\return Error code
 * 	\retval 0 No error
 */
int32_t GPIO_Pin_STM32::read(uint8_t* state) {
	*state = (uint8_t)HAL_GPIO_ReadPin(gpio_port, gpio_pin);
	return 0;
}

/**	\brief Write GPIO pin state
 *  \details Writes STM32 digital pin state using HAL Library
 * 	\param[in] state State to write to pin
 * 	\return Error code
 * 	\retval 0 No error
 */
int32_t GPIO_Pin_STM32::write(uint8_t state) {
	HAL_GPIO_WritePin(gpio_port, gpio_pin, (GPIO_PinState)state);
	return 0;
}

int32_t GPIO_Pin_STM32::set() {
	HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_SET);
	return 0;
}

int32_t GPIO_Pin_STM32::reset() {
	HAL_GPIO_WritePin(gpio_port, gpio_pin, GPIO_PIN_RESET);
	return 0;
}

int32_t GPIO_Pin_STM32::toggle() {
	HAL_GPIO_TogglePin(gpio_port, gpio_pin);
	return 0;
}

GPIO_TypeDef* GPIO_Pin_STM32::getPort() {
	return gpio_port;
}

uint16_t GPIO_Pin_STM32::getPin() {
	return gpio_pin;
}

void GPIO_Pin_STM32::extiDispatch(uint16_t GPIO_Pin){
	// TODO Protect array with mutex
	GPIO_Pin_STM32* object = pinObjects[gpioPinToPinNumber(GPIO_Pin)];
	if(!object){
		printf("EXTI called for uninitialized GPIO_Pin_STM32 %u/n", gpioPinToPinNumber(GPIO_Pin));
		return;
	}
	if(!object->irqObject){
		printf("EXTI called for GPIO_Pin_STM32 %u without callback registered/n", object->pinNumber);
		return;
	}
	object->irqObject->irqHandler(object);
}

/**
 * @brief Registers interruptible object that will handle EXTI
 *
 * @param[in] object Interruptible object which will handle EXTI
 * @return Error code
 * @retval 0 Success
 * @retval -1 Callback previously registered
 */
int32_t GPIO_Pin_STM32::registerExtiCallback(Interruptible* object){
	if(pinObjects[pinNumber] || irqObject){
		// Already registered
		return -1;
	}
	pinObjects[pinNumber] = this;
	irqObject = object;
	return 0;
}

/**
 * @brief Deregisters EXTI handler object
 *
 * @return Error code
 * @retval 0 Success
 */
int32_t GPIO_Pin_STM32::deregisterExtiCallback(){
	pinObjects[pinNumber] = nullptr;
	irqObject = nullptr;
	return 0;
}

// Private methods

int8_t GPIO_Pin_STM32::gpioPinToPinNumber(uint16_t GPIO_Pin) {
	assert_param(IS_GPIO_PIN(GPIO_Pin));
	switch(GPIO_Pin){
	case GPIO_PIN_0:
		return 0;
	case GPIO_PIN_1:
		return 1;
	case GPIO_PIN_2:
		return 2;
	case GPIO_PIN_3:
		return 3;
	case GPIO_PIN_4:
		return 4;
	case GPIO_PIN_5:
		return 5;
	case GPIO_PIN_6:
		return 6;
	case GPIO_PIN_7:
		return 7;
	case GPIO_PIN_8:
		return 8;
	case GPIO_PIN_9:
		return 9;
	case GPIO_PIN_10:
		return 10;
	case GPIO_PIN_11:
		return 11;
	case GPIO_PIN_12:
		return 12;
	case GPIO_PIN_13:
		return 13;
	case GPIO_PIN_14:
		return 14;
	case GPIO_PIN_15:
		return 15;
	default:
		// TODO implement error handling here
		return -1;
	}
}
