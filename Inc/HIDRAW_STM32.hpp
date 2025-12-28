/*
 * HIDRAW_STM32.hpp
 *
 *  Created on: Dec 27, 2025
 *      Author: Gabriel
 */

#ifndef SHARED_ROBOFRAMEWORK_STM32_SRC_HIDRAW_STM32_HPP_
#define SHARED_ROBOFRAMEWORK_STM32_SRC_HIDRAW_STM32_HPP_

#include "HIDRAW.hpp"
#include "main.h"
#include <FreeRTOS.h>

class HIDRAW_STM32 : public HIDRAW {
public:
	HIDRAW_STM32();
	virtual ~HIDRAW_STM32();
	static HIDRAW_STM32* getInstance();
	int32_t init();
	int32_t enqueueInPacket(uint8_t* buffer, uint32_t size);
	int32_t receiveOutPacket(uint8_t* buffer, uint32_t maxSize);
};

#endif /* SHARED_ROBOFRAMEWORK_STM32_SRC_HIDRAW_STM32_HPP_ */
