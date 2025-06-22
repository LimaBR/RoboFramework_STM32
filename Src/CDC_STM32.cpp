/*
 * CDC_STM32.cpp
 *
 *  Created on: Jun 21, 2025
 *      Author: Gabriel
 */

#include "CDC_STM32.hpp"
#include "usbd_cdc_if.h"

CDC_STM32 CDC_STM32_Singleton;

extern "C" {
	int32_t usbCdcReceiveCallback(uint8_t* buffer, uint32_t size){
		return CDC_STM32_Singleton.receiveCallback(buffer, size);
	}
}

CDC_STM32::CDC_STM32() {

}

CDC_STM32::~CDC_STM32() {

}

CDC_STM32* CDC_STM32::getInstance(){
	return &CDC_STM32_Singleton;
}

int32_t CDC_STM32::init() {
	int32_t errors = 0;
	usbRecvBuffer = xStreamBufferCreateStatic(usbRecvBufferSize, 1, usbRecvBufferStorageArea, &usbRecvBufferStaticStructure);
	if(!usbRecvBuffer){
		errors--;
	}
	errors += SharedResource::init("CDC_STM32_Singleton");
	return 0;
}

int32_t CDC_STM32::transmit(uint8_t *buffer, uint32_t size) {
	if(CDC_Transmit_FS(buffer, size) == USBD_OK){
		return 0;
	}else{
		return -1;
	}
}

int32_t CDC_STM32::receiveUntilLineIdle(uint8_t *buffer, uint32_t maxSize) {
	if(!usbRecvBuffer){
		// Not initialized
		return -1;
	}
	uint32_t bytesReceivedTotal = 0;
	uint32_t bytesReceivedThisIteration = xStreamBufferReceive(usbRecvBuffer, buffer, maxSize, portMAX_DELAY);
	maxSize -= bytesReceivedThisIteration;
	bytesReceivedTotal += bytesReceivedThisIteration;
	buffer += bytesReceivedThisIteration;
	while(maxSize){
		bytesReceivedThisIteration = xStreamBufferReceive(usbRecvBuffer, buffer, maxSize, 2);
		if(!bytesReceivedThisIteration){
			// Timeout
			break;
		}
		maxSize -= bytesReceivedThisIteration;
		bytesReceivedTotal += bytesReceivedThisIteration;
		buffer += bytesReceivedThisIteration;
	}
	return bytesReceivedTotal;
}

int32_t CDC_STM32::receiveCallback(uint8_t *buffer, uint32_t size) {
	BaseType_t xHigherPriorityTaskWoken = false;
	if(usbRecvBuffer){
		xStreamBufferSendFromISR(usbRecvBuffer, buffer, size, &xHigherPriorityTaskWoken);
	}
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	return 0;
}
