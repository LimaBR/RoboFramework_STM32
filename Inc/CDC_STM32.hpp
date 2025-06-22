/*
 * CDC_STM32.hpp
 *
 *  Created on: Jun 21, 2025
 *      Author: Gabriel
 */

#ifndef SHARED_ROBOFRAMEWORK_STM32_SRC_CDC_STM32_HPP_
#define SHARED_ROBOFRAMEWORK_STM32_SRC_CDC_STM32_HPP_

#include "main.h"
#include "UART.hpp"
#include "FreeRTOS.h"
#include "stream_buffer.h"

class CDC_STM32 : public UART {
public:
	CDC_STM32();
	virtual ~CDC_STM32();
	static CDC_STM32* getInstance();
	int32_t init();
	int32_t transmit(uint8_t* buffer, uint32_t size);
	int32_t receiveUntilLineIdle(uint8_t* buffer, uint32_t maxSize);
	int32_t receiveCallback(uint8_t* buffer, uint32_t size);
private:
	static constexpr uint32_t usbRecvBufferSize = 256;
	StreamBufferHandle_t usbRecvBuffer = nullptr;
	uint8_t usbRecvBufferStorageArea[usbRecvBufferSize+1];
	StaticStreamBuffer_t usbRecvBufferStaticStructure;
};

#endif /* SHARED_ROBOFRAMEWORK_STM32_SRC_CDC_STM32_HPP_ */
