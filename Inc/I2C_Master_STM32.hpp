/*
 * I2C_Master_STM32.hpp
 *
 *  Created on: May 7, 2024
 *      Author: Gabriel
 */

#ifndef SRC_COMPONENTS_I2C_MASTER_STM32_HPP_
#define SRC_COMPONENTS_I2C_MASTER_STM32_HPP_

#include "main.h"
#include "I2C_Master.hpp"

#include <FreeRTOS.h>
#include <semphr.h>
#include <event_groups.h>

class I2C_Master_STM32 : public I2C_Master {
public:
	I2C_Master_STM32(I2C_HandleTypeDef* hi2c, uint32_t timeout);
	virtual ~I2C_Master_STM32();
	int32_t init();
	int32_t take(uint32_t timeout);
	int32_t give();
	int32_t transmit(uint8_t devAddr, uint8_t* buffer, uint32_t length);
	int32_t receive(uint8_t devAddr, uint8_t* buffer, uint32_t length);
	int32_t regTransmit(uint8_t devAddr, uint8_t regAddr, uint8_t* buffer, uint32_t length);
	int32_t regReceive(uint8_t devAddr, uint8_t regAddr, uint8_t* buffer, uint32_t length);
	int32_t reg16Transmit(uint8_t devAddr, uint16_t regAddr, uint8_t* buffer, uint32_t length);
	int32_t reg16Receive(uint8_t devAddr, uint16_t regAddr, uint8_t* buffer, uint32_t length);
private:
	enum class Event : uint32_t{
		TIMEOUT = 0,
		MASTER_TX_CPLT = 1<<0,
		MASTER_RX_CPLT = 1<<1,
		MEM_TX_CPLT = 1<<2,
		MEM_RX_CPLT = 1<<3,
		ERROR = 1<<4,
	};
	static void masterTxCpltCallbackStatic(I2C_HandleTypeDef* _hi2c);
	static void masterRxCpltCallbackStatic(I2C_HandleTypeDef* _hi2c);
	static void memTxCpltCallbackStatic(I2C_HandleTypeDef* _hi2c);
	static void memRxCpltCallbackStatic(I2C_HandleTypeDef* _hi2c);
	static void errorCallbackStatic(I2C_HandleTypeDef* _hi2c);
	static void dispatchStaticToCallback(I2C_HandleTypeDef* _hi2c, void (I2C_Master_STM32::*callback)(void));
	void masterTxCpltCallback();
	void masterRxCpltCallback();
	void memTxCpltCallback();
	void memRxCpltCallback();
	void errorCallback();
	inline static I2C_Master_STM32* objects[3];
	I2C_HandleTypeDef* hi2c;
	uint32_t timeout;
	SemaphoreHandle_t hi2cMutex;
	StaticSemaphore_t hi2cMutexBuffer;
	EventGroupHandle_t eventGroupHandle;
	StaticEventGroup_t eventGroupBuffer;
	TaskHandle_t taskLocking = nullptr;
};

#endif /* SRC_COMPONENTS_I2C_MASTER_STM32_HPP_ */
