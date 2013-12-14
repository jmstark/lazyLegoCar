#include "observer.h"

namespace obs{
	Observer::Observer(rasp_sock::RaspberrySocket *sock, pipe::USBPipe *pipe){
		this->sock = sock;
		this->pipe = pipe;
	}
	
	
	Observer::~Observer(){
	
	}
	
	
	void run(){
		std::string str;
		while(1){
			if(toArduino.changed.load(std::memory_order_relaxed)){
				toArduino.mtx.lock();
				pipe->usbWrite((void*)&toArduino.com, sizeof com);
				toArduino.mtx.unlock();
			}
			if(fromArduino.changed.load(std::memory_order_relaxed)){
				fromArduino.mtx.lock();
				str = createJSON(&fromArduino.com);
				fromArduino.mtx.unlock();
				sock->sendMsg(str);
			}
		}
	}
};