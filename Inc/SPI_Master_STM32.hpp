/*
 * SPI_Master_STM32.hpp
 *
 *  Created on: Feb 10, 2024
 *      Author: Gabriel
 */

#ifndef SRC_COMPONENTS_SPI_MASTER_STM32_HPP_
#define SRC_COMPONENTS_SPI_MASTER_STM32_HPP_

#include "main.h"
#include <FreeRTOS.h>
#include <semphr.h>
#include <event_groups.h>
#include <SPI_Master.hpp>

class SPI_Master_STM32 : public SPI_Master {
public:
	SPI_Master_STM32(SPI_HandleTypeDef* hspi, uint32_t timeout);
	virtual ~SPI_Master_STM32();
	int32_t init();
	int32_t take(uint32_t timeout);
	int32_t give();
	int32_t transmit(uint8_t* buffer, uint32_t length);
	int32_t receive(uint8_t* buffer, uint32_t length);
	int32_t trx(uint8_t* txBuffer, uint8_t* rxBuffer, uint32_t length);
private:
	enum class Event{
		TX_CPLT = 1<<0,
		RX_CPLT = 1<<1,
		TRX_CPLT = 1<<2,
	};
	static void transmitCallbackStatic(SPI_HandleTypeDef* _hspi);
	static void receiveCallbackStatic(SPI_HandleTypeDef* _hspi);
	static void trxCallbackStatic(SPI_HandleTypeDef* _hspi);
	void transmitCallback();
	void receiveCallback();
	void trxCallback();
	static SPI_Master_STM32* objects[5];
	SPI_HandleTypeDef* hspi;
	uint32_t timeout;
	SemaphoreHandle_t hSpiMutex;
	StaticSemaphore_t spiMutexBuffer;
	EventGroupHandle_t hSpiEventGroup;
	StaticEventGroup_t spiEventGroupBuffer;
	TaskHandle_t taskLocking = nullptr;
};

#endif /* SRC_COMPONENTS_SPI_MASTER_STM32_HPP_ */
