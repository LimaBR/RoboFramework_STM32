/*
 * UART_STM32.hpp
 *
 *  Created on: Aug 17, 2024
 *      Author: Gabriel
 */

#ifndef SRC_COMPONENTS_UART_STM32_HPP_
#define SRC_COMPONENTS_UART_STM32_HPP_

#include "main.h"
#include "UART.hpp"
#include <FreeRTOS.h>
#include <event_groups.h>

class UART_STM32 : public UART {
public:
	UART_STM32(UART_HandleTypeDef* huart);
	virtual ~UART_STM32();
	int32_t init();
	int32_t transmit(uint8_t* buffer, uint32_t size);
	int32_t receiveUntilLineIdle(uint8_t* buffer, uint32_t maxSize);
private:
	enum class Event : uint32_t{
		TIMEOUT = 0,
		TX_CPLT = 1<<0,
		RX_CPLT = 1<<1,
		IDLE = 1<<2,
		ERROR = 1<<3,
	};
	static void txCpltCallbackStatic(UART_HandleTypeDef* _huart);
	static void rxCpltCallbackStatic(UART_HandleTypeDef* _huart);
	static void errorCallbackStatic(UART_HandleTypeDef* _huart);
	static void rxEventCallbackStatic(UART_HandleTypeDef* _huart, uint16_t pos);
	static void dispatchStaticToCallback(UART_HandleTypeDef* _huart, void (UART_STM32::*callback)(void));
	void txCpltCallback();
	void rxCpltCallback();
	void errorCallback();
	void rxEventCallback();
	inline static UART_STM32* objects[10];
	UART_HandleTypeDef* huart;
	EventGroupHandle_t eventGroupHandle;
	StaticEventGroup_t eventGroupBuffer;
	uint32_t rxToIdleRecvBytes;
};

#endif /* SRC_COMPONENTS_UART_STM32_HPP_ */
