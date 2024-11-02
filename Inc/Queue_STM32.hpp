/*
 * Queue.hpp
 *
 *  Created on: Oct 16, 2024
 *      Author: Gabriel
 */

#ifndef SRC_COMPONENTS_QUEUE_STM32_HPP_
#define SRC_COMPONENTS_QUEUE_STM32_HPP_

#include "Queue.hpp"
#include <FreeRTOS.h>
#include <queue.h>
#include <cstring>
#include <cerrno>

template <typename T, uint32_t length> class Queue_STM32 : public Queue<T> {
public:

	Queue_STM32(const char* name) {
		strncpy(this->name, name, 16);
	}

	virtual ~Queue_STM32() {

	}

	int32_t init() {
		handle = xQueueCreateStatic(length, sizeof(T*), storageArea, &structure);
		if(handle){
			vQueueAddToRegistry(handle, name);
			initialized = true;
			return 0;
		}else{
			return -1;
		}
	}

	int32_t send(const T& dataToSend, int32_t timeoutMs) {
		if(!initialized){
			return -EAGAIN;
		}
		T* ptrToSend = new T(dataToSend);
		uint32_t status = xQueueSend(handle, &ptrToSend, timeoutMs);
		if(status == pdTRUE){
			return 0;
		}else{
			delete ptrToSend;
			return -ETIMEDOUT;
		}
	}

	int32_t receive(T* dataToReceive, int32_t timeoutMs) {
		if(!initialized){
			return -EAGAIN;
		}
		T* ptrToRecv;
		uint32_t status = xQueueReceive(handle, &ptrToRecv, timeoutMs);
		if(status == pdTRUE){
			*dataToReceive = *ptrToRecv;
			delete ptrToRecv;
			return 0;
		}else{
			return -ETIMEDOUT;
		}
	}

	char* getName(){
		return name;
	}

	int32_t setName(char* name){
		strncpy(this->name, name, 16);
		return 0;
	}

private:
	QueueHandle_t handle;
	StaticQueue_t structure;
	bool initialized = false;
	char name[17];
	uint8_t storageArea[length*sizeof(T*)];
};

#endif /* SRC_COMPONENTS_QUEUE_STM32_HPP_ */
