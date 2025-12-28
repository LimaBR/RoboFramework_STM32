/*
 * HIDRAW_STM32.cpp
 *
 *  Created on: Dec 27, 2025
 *      Author: Gabriel
 */

#include "HIDRAW_STM32.hpp"
#include "usb_device.h"
#include "usbd_customhid.h"

HIDRAW_STM32 HIDRAW_STM32_Singleton;

extern USBD_HandleTypeDef hUsbDeviceFS;

HIDRAW_STM32::HIDRAW_STM32() {

}

HIDRAW_STM32::~HIDRAW_STM32() {

}

HIDRAW_STM32* HIDRAW_STM32::getInstance(){
	return &HIDRAW_STM32_Singleton;
}

int32_t HIDRAW_STM32::init() {
	int32_t errors = 0;
	errors += SharedResource::init("HIDRAW_STM32_Singleton");
	return errors;
}

int32_t HIDRAW_STM32::enqueueInPacket(const uint8_t *buffer, uint32_t size) {
	int32_t errors = 0;
	if(!taken()){
		// Not taken
		return -1;
	}
	if(size > CUSTOM_HID_EPIN_SIZE){
		// size > endpoint size (would result in fragmented packets)
		return -1;
	}
	// Copying is necessary here because STM32 HAL doesn't receive buffer as const
	// USB FS doesn't use DMA, so after USBD_CUSTOM_HID_SendReport returns, we can change the buffer sent on last call
	memcpy(pktBuffer, buffer, size);
	uint8_t result = USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, pktBuffer, size);
	if(result != USBD_OK){
		errors--;
	}
	return errors;
}

int32_t HIDRAW_STM32::receiveOutPacket(uint8_t *buffer, uint32_t maxSize) {
	int32_t errors = 0;
	if(!taken()){
		return -1;
	}
	return errors;
}
