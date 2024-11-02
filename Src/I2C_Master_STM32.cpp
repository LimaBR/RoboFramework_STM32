/*
 * I2C_Master_STM32.cpp
 *
 *  Created on: May 7, 2024
 *      Author: Gabriel
 */

#include "I2C_Master_STM32.hpp"
#include <cstdio>

I2C_Master_STM32::I2C_Master_STM32(I2C_HandleTypeDef* hi2c, uint32_t timeout) :
		hi2c(hi2c), timeout(timeout) {

}

I2C_Master_STM32::~I2C_Master_STM32() {

}

// Public methods

int32_t I2C_Master_STM32::init() {
	// TODO Implement self test routine
	hi2cMutex = xSemaphoreCreateMutexStatic(&hi2cMutexBuffer);
	vQueueAddToRegistry(hi2cMutex, "hi2cMutex");
	eventGroupHandle = xEventGroupCreateStatic(&eventGroupBuffer);
	HAL_I2C_RegisterCallback(hi2c, HAL_I2C_MASTER_TX_COMPLETE_CB_ID, masterTxCpltCallbackStatic);
	HAL_I2C_RegisterCallback(hi2c, HAL_I2C_MASTER_RX_COMPLETE_CB_ID, masterRxCpltCallbackStatic);
	HAL_I2C_RegisterCallback(hi2c, HAL_I2C_MEM_TX_COMPLETE_CB_ID, memTxCpltCallbackStatic);
	HAL_I2C_RegisterCallback(hi2c, HAL_I2C_MEM_RX_COMPLETE_CB_ID, memRxCpltCallbackStatic);
	HAL_I2C_RegisterCallback(hi2c, HAL_I2C_ERROR_CB_ID, errorCallbackStatic);
#ifdef I2C1
	if(hi2c->Instance == I2C1){
		objects[0] = this;
	}
#endif
#ifdef I2C2
	if(hi2c->Instance == I2C2){
		objects[1] = this;
	}
#endif
#ifdef I2C3
	if(hi2c->Instance == I2C3){
		objects[2] = this;
	}
#endif
	return 0;
}

int32_t I2C_Master_STM32::take(uint32_t timeout) {
	if(xSemaphoreTake(hi2cMutex, timeout)){
		// Took
		taskLocking = xTaskGetCurrentTaskHandle();
		return 0;
	}else{
		// Timeout
		return -1;
	}
}

int32_t I2C_Master_STM32::give() {
	taskLocking = nullptr;
	xSemaphoreGive(hi2cMutex);
	return 0;
}

int32_t I2C_Master_STM32::transmit(uint8_t devAddr, uint8_t *buffer, uint32_t length) {
	if (taskLocking != xTaskGetCurrentTaskHandle()){
		// Not taken
		return -2;
	}
	HAL_I2C_Master_Transmit_DMA(hi2c, devAddr, buffer, length);
	Event event = (Event)xEventGroupWaitBits(eventGroupHandle, 0xFFFFFF, true, false, timeout);
	if(event != Event::MASTER_TX_CPLT){
		// Error
		return -1;
	}
	return 0;
}

int32_t I2C_Master_STM32::receive(uint8_t devAddr, uint8_t *buffer, uint32_t length) {
	if (taskLocking != xTaskGetCurrentTaskHandle()){
		// Not taken
		return -2;
	}
	HAL_I2C_Master_Receive_DMA(hi2c, devAddr, buffer, length);
	Event event = (Event)xEventGroupWaitBits(eventGroupHandle, 0xFFFFFF, true, false, timeout);
	if(event != Event::MASTER_TX_CPLT){
		// Error
		return -1;
	}
	return 0;
}

int32_t I2C_Master_STM32::regTransmit(uint8_t devAddr, uint8_t regAddr, uint8_t *buffer, uint32_t length) {

}

int32_t I2C_Master_STM32::regReceive(uint8_t devAddr, uint8_t regAddr, uint8_t *buffer, uint32_t length) {
}

int32_t I2C_Master_STM32::reg16Transmit(uint8_t devAddr, uint16_t regAddr, uint8_t *buffer, uint32_t length) {
}

int32_t I2C_Master_STM32::reg16Receive(uint8_t devAddr, uint16_t regAddr, uint8_t *buffer, uint32_t length) {
}

// Private methods

void I2C_Master_STM32::masterTxCpltCallbackStatic(I2C_HandleTypeDef* _hi2c) {
	dispatchStaticToCallback(_hi2c, &I2C_Master_STM32::masterTxCpltCallback);
}

void I2C_Master_STM32::masterRxCpltCallbackStatic(I2C_HandleTypeDef* _hi2c) {
	dispatchStaticToCallback(_hi2c, &I2C_Master_STM32::masterRxCpltCallback);
}

void I2C_Master_STM32::memTxCpltCallbackStatic(I2C_HandleTypeDef* _hi2c) {
	dispatchStaticToCallback(_hi2c, &I2C_Master_STM32::memTxCpltCallback);
}

void I2C_Master_STM32::memRxCpltCallbackStatic(I2C_HandleTypeDef* _hi2c) {
	dispatchStaticToCallback(_hi2c, &I2C_Master_STM32::memRxCpltCallback);
}

void I2C_Master_STM32::errorCallbackStatic(I2C_HandleTypeDef* _hi2c) {
	dispatchStaticToCallback(_hi2c, &I2C_Master_STM32::errorCallback);
}

void I2C_Master_STM32::dispatchStaticToCallback(I2C_HandleTypeDef* _hi2c, void (I2C_Master_STM32::*callback)(void)) {
#ifdef I2C1
	if(_hi2c->Instance == I2C1){
		if(objects[0]){
			(objects[0]->*callback)();
			return;
		}
		printf("IRQ called for I2C1 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef I2C2
	if(_hi2c->Instance == I2C2){
		if(objects[1]){
			(objects[1]->*callback)();
			return;
		}
		printf("IRQ called for I2C2 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef I2C3
	if(_hi2c->Instance == I2C3){
		if(objects[2]){
			(objects[2]->*callback)();
			return;
		}
		printf("IRQ called for I2C2 with uninitialized object\n");
		Error_Handler();
	}
#endif
	printf("Unknown I2C instance %p\n", (void*)_hi2c);
	Error_Handler();
}

void I2C_Master_STM32::masterTxCpltCallback() {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xEventGroupSetBitsFromISR(eventGroupHandle, (EventBits_t)Event::MASTER_TX_CPLT, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void I2C_Master_STM32::masterRxCpltCallback() {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xEventGroupSetBitsFromISR(eventGroupHandle, (EventBits_t)Event::MASTER_RX_CPLT, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void I2C_Master_STM32::memTxCpltCallback() {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xEventGroupSetBitsFromISR(eventGroupHandle, (EventBits_t)Event::MEM_TX_CPLT, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void I2C_Master_STM32::memRxCpltCallback() {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xEventGroupSetBitsFromISR(eventGroupHandle, (EventBits_t)Event::MEM_RX_CPLT, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void I2C_Master_STM32::errorCallback() {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xEventGroupSetBitsFromISR(eventGroupHandle, (EventBits_t)Event::ERROR, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
