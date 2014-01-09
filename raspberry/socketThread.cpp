#include "socketThread.h"

void socketThreadEntry(rasp_sock::RaspberrySocket *sPtr, comSync *cPtr, Observer* o){
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
		std::cout<<str<<std::endl;
		if(str.compare(COMMAND_FWD) == 0){
			//o->drive(DIR_FWD);
			cPtr->mtx.lock();
			cPtr->comc.direction = 1;
			cPtr->mtx.unlock();
			cPtr->changed.store(true, std::memory_order_relaxed);
		}
		else if(str.compare(COMMAND_RWD) == 0){
			//o->drive(DIR_RWD);
			cPtr->mtx.lock();
			cPtr->comc.direction = -1;
			cPtr->mtx.unlock();
			cPtr->changed.store(true, std::memory_order_relaxed);
		}
		else if(str.compare(COMMAND_LEFT) == 0){
			//o->steer(LEFT);
			cPtr->mtx.lock();
			cPtr->comc.steering = -1;
			cPtr->mtx.unlock();
			cPtr->changed.store(true, std::memory_order_relaxed);
		}
		else if(str.compare(COMMAND_RIGHT) == 0){
			//o->steer(RIGHT);
			cPtr->mtx.lock();
			cPtr->comc.steering = 1;
			cPtr->mtx.unlock();
			cPtr->changed.store(true, std::memory_order_relaxed);
		}
		else if(str.compare(COMMAND_STRAIGHT) == 0){
			//o->steer(STRAIGHT);
			cPtr->mtx.lock();
			cPtr->comc.steering = 0;
			cPtr->mtx.unlock();
			cPtr->changed.store(true, std::memory_order_relaxed);
		}
		else if(str.compare(COMMAND_STOP) == 0){
			//o->drive(DIR_STOP);
			cPtr->mtx.lock();
			cPtr->comc.direction = 0;
			cPtr->mtx.unlock();
			cPtr->changed.store(true, std::memory_order_relaxed);
		}
		else if(1==0){
			cPtr->mtx.lock();
			cPtr->comc.speed = std::stoi(str);
			cPtr->mtx.unlock();
			cPtr->changed.store(true, std::memory_order_relaxed);
		}
	}
}
