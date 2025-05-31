/*
 * SoftTimer_STM32.cpp
 *
 *  Created on: May 24, 2025
 *      Author: Gabriel
 */

#include <SoftTimer_STM32.hpp>
#include <cstring>
#include <cerrno>
#include <cstdio>

SoftTimer_STM32::SoftTimer_STM32(Interruptible* irqObject, const char *name) :
		irqObject(irqObject)
{
	if(name){
		setName(name);
	}
}

SoftTimer_STM32::~SoftTimer_STM32() {

}

int32_t SoftTimer_STM32::init() {
	hTimer = xTimerCreateStatic(name, 1, false, this, timerCalback, &pxTimerBuffer);
	hEventGroup = xEventGroupCreateStatic(&eventGroupStaticMemory);
	initialized = true;
	return 0;
}

int32_t SoftTimer_STM32::start(uint32_t periodInTicks, bool autoReload) {
	if(!initialized){
		return -EAGAIN;
	}
	vTimerSetReloadMode(hTimer, autoReload);
	if(xTimerChangePeriod(hTimer, periodInTicks, 0) == pdPASS){
		return 0;
	}else{
		return -EWOULDBLOCK;
	}
}

int32_t SoftTimer_STM32::stop() {
	if(!initialized){
		return -EAGAIN;
	}
	if(xTimerStop(hTimer, 0)){
		return 0;
	}else{
		return -EWOULDBLOCK;
	}
}

int32_t SoftTimer_STM32::waitForCompletion(uint32_t timeoutInTicks) {
	if(!initialized){
		return -EAGAIN;
	}
	if(xEventGroupWaitBits(hEventGroup, 1<<0, true, false, timeoutInTicks) == 1<<0){
		return 0;
	}else{
		return -ETIMEDOUT;
	}
}

char* SoftTimer_STM32::getName() {
	return name;
}

int32_t SoftTimer_STM32::setName(const char *name) {
	strncpy(this->name, name, 16);
	return 0;
}

void SoftTimer_STM32::timerCalback(TimerHandle_t xTimer) {
	SoftTimer_STM32* object = static_cast<SoftTimer_STM32*>(pvTimerGetTimerID(xTimer));
	if(!object->irqObject){
		printf("SoftTimer: %p expired with null object\n", object);
		return;
	}
	object->irqObject->irqHandler(object);
	xEventGroupSetBits(object->hEventGroup, 1<<0);
}
