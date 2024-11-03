/*
 * SPI_Master_STM32.cpp
 *
 *  Created on: Feb 10, 2024
 *      Author: Gabriel
 */

#include <SPI_Master_STM32.hpp>

SPI_Master_STM32* SPI_Master_STM32::objects[];

SPI_Master_STM32::SPI_Master_STM32(SPI_HandleTypeDef* hspi, uint32_t timeout) :
	hspi(hspi), timeout(timeout) {

}

SPI_Master_STM32::~SPI_Master_STM32() {

}

// Public methods

int32_t SPI_Master_STM32::init(){
	// TODO Implement self test routine
	int32_t errors = 0;
	errors += SharedResource::init();
	hSpiEventGroup = xEventGroupCreateStatic(&spiEventGroupBuffer);
	HAL_SPI_RegisterCallback(hspi, HAL_SPI_TX_COMPLETE_CB_ID, transmitCallbackStatic);
	HAL_SPI_RegisterCallback(hspi, HAL_SPI_RX_COMPLETE_CB_ID, receiveCallbackStatic);
	HAL_SPI_RegisterCallback(hspi, HAL_SPI_TX_RX_COMPLETE_CB_ID, trxCallbackStatic);
#ifdef SPI1
	if(hspi->Instance == SPI1){
		objects[0] = this;
	}
#endif
#ifdef SPI2
	if(hspi->Instance == SPI2){
		objects[1] = this;
	}
#endif
#ifdef SPI3
	if(hspi->Instance == SPI3){
		objects[2] = this;
	}
#endif
#ifdef SPI4
	if(hspi->Instance == SPI4){
		objects[3] = this;
	}
#endif
#ifdef SPI5
	if(hspi->Instance == SPI5){
		objects[4] = this;
	}
#endif
	return errors;
}

int32_t SPI_Master_STM32::transmit(uint8_t *buffer, uint32_t length) {
	if (!taken()){
		// Not taken
		return -1;
	}
	HAL_SPI_Transmit_DMA(hspi, buffer, length);
	EventBits_t bits = xEventGroupWaitBits(hSpiEventGroup, (EventBits_t)Event::TX_CPLT, true, true, timeout);
	if(!bits){
		// Timeout
		return -1;
	}
	return 0;
}

int32_t SPI_Master_STM32::receive(uint8_t *buffer, uint32_t length) {
	if (!taken()){
		// Not taken
		return -1;
	}
	HAL_SPI_Receive_DMA(hspi, buffer, length);
	EventBits_t bits = xEventGroupWaitBits(hSpiEventGroup, (EventBits_t)Event::RX_CPLT, true, true, timeout);
	if(!bits){
		// Timeout
		return -1;
	}
	return 0;
}

int32_t SPI_Master_STM32::trx(uint8_t *txBuffer, uint8_t *rxBuffer, uint32_t length) {
	if (!taken()){
		// Not taken
		return -1;
	}
	HAL_SPI_TransmitReceive_DMA(hspi, txBuffer, rxBuffer, length);
	EventBits_t bits = xEventGroupWaitBits(hSpiEventGroup, (EventBits_t)Event::TRX_CPLT, true, true, timeout);
	if(!bits){
		// Timeout
		return -1;
	}
	return 0;
}

// Private methods

void SPI_Master_STM32::transmitCallbackStatic(SPI_HandleTypeDef *_hspi) {
#ifdef SPI1
	if(_hspi->Instance == SPI1){
		if(objects[0]){
			objects[0]->transmitCallback();
		}
	}
#endif
#ifdef SPI2
	if(_hspi->Instance == SPI2){
		if(objects[1]){
			objects[1]->transmitCallback();
		}
	}
#endif
#ifdef SPI3
	if(_hspi->Instance == SPI3){
		if(objects[2]){
			objects[2]->transmitCallback();
		}
	}
#endif
#ifdef SPI4
	if(_hspi->Instance == SPI4){
		if(objects[3]){
			objects[3]->transmitCallback();
		}
	}
#endif
#ifdef SPI5
	if(_hspi->Instance == SPI5){
		if(objects[4]){
			objects[4]->transmitCallback();
		}
	}
#endif
}

void SPI_Master_STM32::receiveCallbackStatic(SPI_HandleTypeDef *_hspi) {
#ifdef SPI1
	if(_hspi->Instance == SPI1){
		if(objects[0]){
			objects[0]->receiveCallback();
		}
	}
#endif
#ifdef SPI2
	if(_hspi->Instance == SPI2){
		if(objects[1]){
			objects[1]->receiveCallback();
		}
	}
#endif
#ifdef SPI3
	if(_hspi->Instance == SPI3){
		if(objects[2]){
			objects[2]->receiveCallback();
		}
	}
#endif
#ifdef SPI4
	if(_hspi->Instance == SPI4){
		if(objects[3]){
			objects[3]->receiveCallback();
		}
	}
#endif
#ifdef SPI5
	if(_hspi->Instance == SPI5){
		if(objects[4]){
			objects[4]->receiveCallback();
		}
	}
#endif
}

void SPI_Master_STM32::trxCallbackStatic(SPI_HandleTypeDef *_hspi) {
#ifdef SPI1
	if(_hspi->Instance == SPI1){
		if(objects[0]){
			objects[0]->trxCallback();
		}
	}
#endif
#ifdef SPI2
	if(_hspi->Instance == SPI2){
		if(objects[1]){
			objects[1]->trxCallback();
		}
	}
#endif
#ifdef SPI3
	if(_hspi->Instance == SPI3){
		if(objects[2]){
			objects[2]->trxCallback();
		}
	}
#endif
#ifdef SPI4
	if(_hspi->Instance == SPI4){
		if(objects[3]){
			objects[3]->trxCallback();
		}
	}
#endif
#ifdef SPI5
	if(_hspi->Instance == SPI5){
		if(objects[4]){
			objects[4]->trxCallback();
		}
	}
#endif
}

void SPI_Master_STM32::transmitCallback() {
	BaseType_t xHigherPriorityTaskWoken = false;
	xEventGroupSetBitsFromISR(hSpiEventGroup, (EventBits_t)Event::TX_CPLT, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void SPI_Master_STM32::receiveCallback() {
	BaseType_t xHigherPriorityTaskWoken = false;
	xEventGroupSetBitsFromISR(hSpiEventGroup, (EventBits_t)Event::RX_CPLT, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void SPI_Master_STM32::trxCallback() {
	BaseType_t xHigherPriorityTaskWoken = false;
	xEventGroupSetBitsFromISR(hSpiEventGroup, (EventBits_t)Event::TRX_CPLT, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
