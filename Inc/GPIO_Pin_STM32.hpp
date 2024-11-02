/*
 * GPIO_Pin_STM32.hpp
 *
 *  Created on: Feb 10, 2024
 *      Author: Gabriel
 */

#ifndef SRC_COMPONENTS_GPIO_PIN_STM32_HPP_
#define SRC_COMPONENTS_GPIO_PIN_STM32_HPP_

#include "main.h"
#include "GPIO_Pin.hpp"

/**	\brief STM32 implementation of GPIO_Pin
 *
 *
 */
class GPIO_Pin_STM32 : public GPIO_Pin {
public:
	GPIO_Pin_STM32(GPIO_TypeDef* gpio_port, uint16_t gpio_pin);
	virtual ~GPIO_Pin_STM32();
	static void extiDispatch(uint16_t GPIO_Pin);
	int32_t init();
	int32_t read(uint8_t* state);
	int32_t write(uint8_t state);
	int32_t set();
	int32_t reset();
	int32_t toggle();
	int32_t registerExtiCallback(Interruptible* object);
	int32_t deregisterExtiCallback();
	GPIO_TypeDef* getPort();
	uint16_t getPin();
private:
	static int8_t gpioPinToPinNumber(uint16_t GPIO_Pin);
	static inline GPIO_Pin_STM32* pinObjects[16];
	GPIO_TypeDef* gpio_port;
	uint16_t gpio_pin;
	int8_t pinNumber = -1;
};

#endif /* SRC_COMPONENTS_GPIO_PIN_STM32_HPP_ */
