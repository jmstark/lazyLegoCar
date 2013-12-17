#include "socketThread.h"

void socketThreadEntry(rasp_sock::RaspberrySocket *sPtr, comSync *cPtr){
	std::string str;
	char command[32];
	int val;
	if(sPtr == NULL || cPtr == NULL){
		fprintf(stderr, "invalid sock-thread args\n");
		return;
	}
	sPtr->waitForClient();
	while(1){
		str = sPtr->receive();
		if(str.compare(COMMAND_FWD) == NULL){
			cPtr->mtx.lock();
			cPtr->comc.direction = 1;
			cPtr->mtx.unlock();
			cPtr->changed.store(true, std::memory_order_relaxed);
		}
		else if(str.compare(COMMAND_RWD) == NULL){
			cPtr->mtx.lock();
			cPtr->comc.direction = -1;
			cPtr->mtx.unlock();
			cPtr->changed.store(true, std::memory_order_relaxed);
		}
		else if(str.compare(COMMAND_LEFT) == NULL){
			cPtr->mtx.lock();
			cPtr->comc.steering = -1;
			cPtr->mtx.unlock();
			cPtr->changed.store(true, std::memory_order_relaxed);
		}
		else if(str.compare(COMMAND_RIGHT) == NULL){
			cPtr->mtx.lock();
			cPtr->comc.steering = 1;
			cPtr->mtx.unlock();
			cPtr->changed.store(true, std::memory_order_relaxed);
		}
		else if(str.compare(COMMAND_STRAIGHT) == NULL){
			cPtr->mtx.lock();
			cPtr->comc.steering = 0;
			cPtr->mtx.unlock();
			cPtr->changed.store(true, std::memory_order_relaxed);
		}
		else if(str.compare(COMMAND_STOP) == NULL){
			cPtr->mtx.lock();
			cPtr->comc.direction = 0;
			cPtr->mtx.unlock();
			cPtr->changed.store(true, std::memory_order_relaxed);
		}
		else{
			cPtr->mtx.lock();
			cPtr->comc.speed = std::stoi(str);
			cPtr->mtx.unlock();
			cPtr->changed.store(true, std::memory_order_relaxed);
		}
	}
}