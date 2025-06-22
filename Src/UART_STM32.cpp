/*
 * UART_STM32.cpp
 *
 *  Created on: Aug 17, 2024
 *      Author: Gabriel
 */

#include "UART_STM32.hpp"
#include <cstdio>

UART_STM32::UART_STM32(UART_HandleTypeDef* huart) : huart(huart){

}

UART_STM32::~UART_STM32() {

}

int32_t UART_STM32::init() {
	int32_t errors = 0;
#ifdef UART1
	if(huart->Instance == UART1){
		objects[0] = this;
	}
#endif
#ifdef UART2
	if(huart->Instance == UART2){
		objects[1] = this;
	}
#endif
#ifdef UART3
	if(huart->Instance == UART3){
		objects[2] = this;
	}
#endif
#ifdef UART4
	if(huart->Instance == UART4){
		objects[3] = this;
	}
#endif
#ifdef UART5
	if(huart->Instance == UART5){
		objects[4] = this;
	}
#endif
#ifdef UART6
	if(huart->Instance == UART6){
		objects[5] = this;
	}
#endif
#ifdef UART7
	if(huart->Instance == UART7){
		objects[6] = this;
	}
#endif
#ifdef UART8
	if(huart->Instance == UART8){
		objects[7] = this;
	}
#endif
#ifdef UART9
	if(huart->Instance == UART9){
		objects[8] = this;
	}
#endif
#ifdef UART10
	if(huart->Instance == UART10){
		objects[9] = this;
	}
#endif
#ifdef USART1
	if(huart->Instance == USART1){
		objects[0] = this;
	}
#endif
#ifdef USART2
	if(huart->Instance == USART2){
		objects[1] = this;
	}
#endif
#ifdef USART3
	if(huart->Instance == USART3){
		objects[2] = this;
	}
#endif
#ifdef USART4
	if(huart->Instance == USART4){
		objects[3] = this;
	}
#endif
#ifdef USART5
	if(huart->Instance == USART5){
		objects[4] = this;
	}
#endif
#ifdef USART6
	if(huart->Instance == USART6){
		objects[5] = this;
	}
#endif
#ifdef USART7
	if(huart->Instance == USART7){
		objects[6] = this;
	}
#endif
#ifdef USART8
	if(huart->Instance == USART8){
		objects[7] = this;
	}
#endif
#ifdef USART9
	if(huart->Instance == USART9){
		objects[8] = this;
	}
#endif
#ifdef USART10
	if(huart->Instance == USART10){
		objects[9] = this;
	}
#endif
	errors += SharedResource::init("UART");
	eventGroupHandle = xEventGroupCreateStatic(&eventGroupBuffer);
	errors -= HAL_UART_RegisterCallback(huart, HAL_UART_TX_COMPLETE_CB_ID, txCpltCallbackStatic);
	errors -= HAL_UART_RegisterCallback(huart, HAL_UART_RX_COMPLETE_CB_ID, rxCpltCallbackStatic);
	errors -= HAL_UART_RegisterCallback(huart, HAL_UART_ERROR_CB_ID, errorCallbackStatic);
	errors -= HAL_UART_RegisterRxEventCallback(huart, rxEventCallbackStatic);
	return errors;
}

int32_t UART_STM32::transmit(uint8_t* buffer, uint32_t size) {
	if(!taken()){
		// Mutex not locked
		return -1;
	}
	if(HAL_UART_Transmit_DMA(huart, buffer, size)){
		return -1;
	}
	Event bits = (Event)xEventGroupWaitBits(eventGroupHandle, (uint32_t)Event::TX_CPLT | (uint32_t)Event::ERROR, true, false, portMAX_DELAY);
	// Only one event can happen
	switch(bits){
	case Event::TX_CPLT:
		return 0;
	case Event::ERROR:
	default:
		return -1;
	}
}

int32_t UART_STM32::receiveUntilLineIdle(uint8_t* buffer, uint32_t maxSize) {
	if(!taken()){
		// Mutex not locked
		return -1;
	}
	if(HAL_UARTEx_ReceiveToIdle_DMA(huart, buffer, maxSize)){
		return -1;
	}
	Event bits = (Event)xEventGroupWaitBits(eventGroupHandle, (uint32_t)Event::RX_CPLT | (uint32_t)Event::IDLE | (uint32_t)Event::ERROR, true, false, portMAX_DELAY);
	// Only one event can happen
	switch(bits){
	case Event::RX_CPLT:
	case Event::IDLE:
		return rxToIdleRecvBytes;
	case Event::ERROR:
	default:
		return -1;
	}
}

void UART_STM32::txCpltCallbackStatic(UART_HandleTypeDef *_huart) {
	dispatchStaticToCallback(_huart, &UART_STM32::txCpltCallback);
}

void UART_STM32::rxCpltCallbackStatic(UART_HandleTypeDef *_huart) {
	dispatchStaticToCallback(_huart, &UART_STM32::rxCpltCallback);
}

void UART_STM32::errorCallbackStatic(UART_HandleTypeDef *_huart) {
	dispatchStaticToCallback(_huart, &UART_STM32::errorCallback);
}

void UART_STM32::rxEventCallbackStatic(UART_HandleTypeDef *_huart, [[maybe_unused]]uint16_t pos) {
	dispatchStaticToCallback(_huart, &UART_STM32::rxEventCallback);
}

void UART_STM32::dispatchStaticToCallback(UART_HandleTypeDef* _huart, void (UART_STM32::*callback)(void)) {
#ifdef UART1
	if(_huart->Instance == UART1){
		if(objects[0]){
			(objects[0]->*callback)();
			return;
		}
		printf("IRQ called for UART1 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef UART2
	if(_huart->Instance == UART2){
		if(objects[1]){
			(objects[1]->*callback)();
			return;
		}
		printf("IRQ called for UART2 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef UART3
	if(_huart->Instance == UART3){
		if(objects[2]){
			(objects[2]->*callback)();
			return;
		}
		printf("IRQ called for UART3 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef UART4
	if(_huart->Instance == UART4){
		if(objects[3]){
			(objects[3]->*callback)();
			return;
		}
		printf("IRQ called for UART4 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef UART5
	if(_huart->Instance == UART5){
		if(objects[4]){
			(objects[4]->*callback)();
			return;
		}
		printf("IRQ called for UART5 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef UART6
	if(_huart->Instance == UART6){
		if(objects[5]){
			(objects[5]->*callback)();
			return;
		}
		printf("IRQ called for UART6 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef UART7
	if(_huart->Instance == UART7){
		if(objects[6]){
			(objects[6]->*callback)();
			return;
		}
		printf("IRQ called for UART7 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef UART8
	if(_huart->Instance == UART8){
		if(objects[7]){
			(objects[7]->*callback)();
			return;
		}
		printf("IRQ called for UART8 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef UART9
	if(_huart->Instance == UART9){
		if(objects[8]){
			(objects[8]->*callback)();
			return;
		}
		printf("IRQ called for UART9 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef UART10
	if(_huart->Instance == UART10){
		if(objects[9]){
			(objects[9]->*callback)();
			return;
		}
		printf("IRQ called for UART10 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef USART1
	if(_huart->Instance == USART1){
		if(objects[0]){
			(objects[0]->*callback)();
			return;
		}
		printf("IRQ called for USART1 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef USART2
	if(_huart->Instance == USART2){
		if(objects[1]){
			(objects[1]->*callback)();
			return;
		}
		printf("IRQ called for USART2 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef USART3
	if(_huart->Instance == USART3){
		if(objects[2]){
			(objects[2]->*callback)();
			return;
		}
		printf("IRQ called for USART3 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef USART4
	if(_huart->Instance == USART4){
		if(objects[3]){
			(objects[3]->*callback)();
			return;
		}
		printf("IRQ called for USART4 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef USART5
	if(_huart->Instance == USART5){
		if(objects[4]){
			(objects[4]->*callback)();
			return;
		}
		printf("IRQ called for USART5 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef USART6
	if(_huart->Instance == USART6){
		if(objects[5]){
			(objects[5]->*callback)();
			return;
		}
		printf("IRQ called for USART6 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef USART7
	if(_huart->Instance == USART7){
		if(objects[6]){
			(objects[6]->*callback)();
			return;
		}
		printf("IRQ called for USART7 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef USART8
	if(_huart->Instance == USART8){
		if(objects[7]){
			(objects[7]->*callback)();
			return;
		}
		printf("IRQ called for USART8 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef USART9
	if(_huart->Instance == USART9){
		if(objects[8]){
			(objects[8]->*callback)();
			return;
		}
		printf("IRQ called for USART9 with uninitialized object\n");
		Error_Handler();
	}
#endif
#ifdef USART10
	if(_huart->Instance == USART10){
		if(objects[9]){
			(objects[9]->*callback)();
			return;
		}
		printf("IRQ called for USART10 with uninitialized object\n");
		Error_Handler();
	}
#endif
	printf("Unknown UART instance %p\n", (void*)_huart);
	Error_Handler();
}

void UART_STM32::txCpltCallback() {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xEventGroupSetBitsFromISR(eventGroupHandle, (EventBits_t)Event::TX_CPLT, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void UART_STM32::rxCpltCallback() {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xEventGroupSetBitsFromISR(eventGroupHandle, (EventBits_t)Event::RX_CPLT, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void UART_STM32::errorCallback() {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xEventGroupSetBitsFromISR(eventGroupHandle, (EventBits_t)Event::ERROR, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void UART_STM32::rxEventCallback() {
	// According to stm32f4xx_hal_uart.c, TC and HT events are handled here if reception is to be until idle.
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	rxToIdleRecvBytes = huart->RxXferSize - huart->RxXferCount;
	switch(HAL_UARTEx_GetRxEventType(huart)){
	case HAL_UART_RXEVENT_TC:
		xEventGroupSetBitsFromISR(eventGroupHandle, (EventBits_t)Event::RX_CPLT, &xHigherPriorityTaskWoken);
		break;
	case HAL_UART_RXEVENT_HT:
		// Handling not implemented yet
		break;
	case HAL_UART_RXEVENT_IDLE:
		xEventGroupSetBitsFromISR(eventGroupHandle, (EventBits_t)Event::IDLE, &xHigherPriorityTaskWoken);
		break;
	default:
		// TODO implement error handling here
		break;
	}
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
