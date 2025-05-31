/*
 * SoftTimer_STM32.hpp
 *
 *  Created on: May 24, 2025
 *      Author: Gabriel
 */

#ifndef SHARED_ROBOFRAMEWORK_STM32_SRC_SOFTTIMER_STM32_HPP_
#define SHARED_ROBOFRAMEWORK_STM32_SRC_SOFTTIMER_STM32_HPP_

#include "SoftTimer.hpp"
#include "InterruptReason.hpp"
#include "Interruptible.hpp"
#include <cinttypes>
#include <FreeRTOS.h>
#include <timers.h>
#include <event_groups.h>

class SoftTimer_STM32 : public SoftTimer {
public:
	SoftTimer_STM32(Interruptible* irqObject, const char* name);
	virtual ~SoftTimer_STM32();
	int32_t init();
	int32_t start(uint32_t periodInTicks, bool autoReload);
	int32_t stop();
	int32_t waitForCompletion(uint32_t timeoutInTicks);
	char* getName();
	int32_t setName(const char* name);
private:
	static void timerCalback(TimerHandle_t xTimer);
	TimerHandle_t hTimer;
	StaticTimer_t pxTimerBuffer;
	EventGroupHandle_t hEventGroup;
	StaticEventGroup_t eventGroupStaticMemory;
	Interruptible* irqObject;
	bool initialized = false;
	char name[17] = "SoftTimer";
};

#endif /* SHARED_ROBOFRAMEWORK_STM32_SRC_SOFTTIMER_STM32_HPP_ */
