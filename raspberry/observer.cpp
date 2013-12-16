#include "observer.h"

namespace obs{
	Observer::Observer(rasp_sock::RaspberrySocket *sock,pipe::USBPipe *pipe){
		this->sock = sock;
		this->pipe = pipe;
	}
	
	
	Observer::~Observer(){
	
	}

	void drive(uint8_t direction)
	{
		pipe.usbWrite(&direction,sizeof(direction));
	}

	void steer(uint8_t direction)
	{
		pipe.usbWrite(&direction,sizeof(direction));
	}

	void moveFrontLaser(uint8_t degrees)
	{
		pipe.usbWrite(&CMD_MOVE_LASER_FRONT,sizeof(uint8_t));
		pipe.usbWrite(&degrees,sizeof(degrees));
	}

	void moveBackLaser(uint8_t degrees)
	{
		pipe.usbWrite(&CMD_MOVE_LASER_BACK,sizeof(uint8_t));
		pipe.usbWrite(&degrees,sizeof(degrees));
	}
		
	void getFrontDistance(uint8_t[] dist)
	{
		pipe.usbWrite(&GET_LASERDATA_BACK,sizeof(uint8_t));
		//TODO: get data
	}

	uint8_t getBackDistance()
	{
		pipe.usbWrite(&GET_LASERDATA_BACK,sizeof(uint8_t));
		//TODO: get data
		return 0;
	}
	
	
	void run(){
		std::string str;
		while(1){
			if(toArduino.changed.load(std::memory_order_relaxed)){
				toArduino.mtx.lock();
				toArduino.changed.store(false, std::memory_order_relaxed);
				pipe->usbWrite((void*)&toArduino.com, sizeof com);
				toArduino.mtx.unlock();
			}
			/*if(fromArduino.changed.load(std::memory_order_relaxed)){
				fromArduino.mtx.lock();
				fromArduino.changed.store(false, std::memory_order_relaxed);
				str = createJSON(&fromArduino.com);
				fromArduino.mtx.unlock();
				sock->sendMsg(str);
			}
			*/
			fromArduino.mtx.lock();
			pipe->usbRead(fromArduino.com.laserData, LASERDATA_LEN);
		}
	}
};
