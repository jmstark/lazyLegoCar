#include "observer.h"

	Observer::Observer(rasp_sock::RaspberrySocket *sock,USBPipe *pipe){
		this->sock = sock;
		this->pipe = pipe;
	}
	
	
	Observer::~Observer(){
	
	}

	void Observer::drive(uint8_t direction)
	{
		pipe->usbWrite((void*)&direction,sizeof(direction));
	}

	void Observer::steer(uint8_t direction)
	{
		pipe->usbWrite((void*)&direction,sizeof(direction));
	}

	void Observer::moveFrontLaser(uint8_t degrees)
	{
		pipe->usbWrite((void*)&CMD_MOVE_LASER_FRONT,sizeof(uint8_t));
		pipe->usbWrite((void*)&degrees,sizeof(degrees));
	}

	void Observer::moveBackLaser(uint8_t degrees)
	{
		pipe->usbWrite((void*)&CMD_MOVE_LASER_BACK,sizeof(uint8_t));
		pipe->usbWrite((void*)&degrees,sizeof(degrees));
	}
		
	void Observer::getFrontDistance(uint8_t* dist)
	{
		pipe->usbFlush();
		pipe->usbWrite((void*)&CMD_GET_LASERDATA_FRONT,sizeof(uint8_t));
		pipe->usbRead(fromArduino.comc.laserDataFront,LASERDATA_LEN,LASERDATA_LEN);
		for(int i=0;i<LASERDATA_LEN;i++)
		{
			dist[i]=fromArduino.comc.laserDataFront[i];
		}
	}

	uint8_t Observer::getBackDistance()
	{
		pipe->usbFlush();
		pipe->usbWrite((void*)&CMD_GET_LASERDATA_BACK,sizeof(uint8_t));
		pipe->usbRead(&fromArduino.comc.laserDataBack,sizeof(uint8_t));
		return fromArduino.comc.laserDataBack;
	}
	
	
	void Observer::run(){
		std::string str;
		while(1){
			if(toArduino.changed.load(std::memory_order_relaxed)){
				std::cout<<"nh"<<std::endl;
				toArduino.mtx.lock();
				toArduino.changed.store(false, std::memory_order_relaxed);
				if(toArduino.comc.direction == -1)
					drive(DIR_RWD);
				if(toArduino.comc.direction == 0)
					drive(DIR_STOP);
				if(toArduino.comc.direction == 1)
					drive(DIR_FWD);
				if(toArduino.comc.steering == -1)
					steer(LEFT);
				if(toArduino.comc.steering == 0)
					steer(STRAIGHT);
				if(toArduino.comc.steering == 1)
					steer(RIGHT);

				

				toArduino.mtx.unlock();
			}
			/*if(fromArduino.changed.load(std::memory_order_relaxed)){
				fromArduino.mtx.lock();
				fromArduino.changed.store(false, std::memory_order_relaxed);
				str = createJSON(&fromArduino.com);
				fromArduino.mtx.unlock();
				sock->sendMsg(str);
			}
			fromArduino.mtx.lock();*/
		}
	}
