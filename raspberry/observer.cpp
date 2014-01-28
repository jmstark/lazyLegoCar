#include "observer.h"

using namespace std;


	Observer::Observer(rasp_sock::RaspberrySocket *sock,USBPipe *pipe){
		this->sock = sock;
		this->pipe = pipe;
	}
	
	
	Observer::~Observer(){
	
	}

	void Observer::drive(uint8_t direction)
	{
#ifdef RASP_DEBUG
		printf("driving to: %d\n", direction);
#endif
		pipe->usbWrite((void*)&direction,sizeof(direction));
	}

	void Observer::steer(uint8_t direction)
	{
#ifdef RASP_DEBUG
		printf("steering to: %d\n", direction);
#endif
		pipe->usbWrite((void*)&direction,sizeof(direction));
	}

	void Observer::moveFrontLaser(uint8_t degrees)
	{
#ifdef RASP_DEBUG
		printf("moving frontLaser: %d\n",degrees);
#endif
		pipe->usbWrite((void*)&CMD_MOVE_LASER_FRONT,sizeof(uint8_t));
		pipe->usbWrite((void*)&degrees,sizeof(degrees));
	}

	void Observer::moveBackLaser(uint8_t degrees)
	{
#ifdef RASP_DEBUG
		printf("moving backLaser: %d\n", degrees);
#endif
		pipe->usbWrite((void*)&CMD_MOVE_LASER_BACK,sizeof(uint8_t));
		pipe->usbWrite((void*)&degrees,sizeof(degrees));
	}
		
	void Observer::getFrontDistance(uint8_t* dist)
	{
#ifdef RASP_DEBUG
		cout << "reading front dist" << endl;
#endif
		pipe->usbFlush();
		pipe->usbWrite((void*)&CMD_GET_LASERDATA_FRONT,sizeof(uint8_t));
		pipe->usbRead(toArduino.comc.laserDataFront,LASERDATA_LEN,LASERDATA_LEN);
		if (dist != NULL){
			for(int i=0;i<LASERDATA_LEN;i++)
			{
				dist[i]=fromArduino.comc.laserDataFront[i];
			}	
		}
	}

	uint8_t Observer::getBackDistance()
	{
#ifdef RASP_DEBUG
		cout << "reading back dist" << endl;
#endif
		pipe->usbFlush();
		pipe->usbWrite((void*)&CMD_GET_LASERDATA_BACK,sizeof(uint8_t));
		pipe->usbRead(&toArduino.comc.laserDataBack,sizeof(uint8_t));
		return toArduino.comc.laserDataBack;
	}
	
	
	void Observer::run(){
		std::string str;
		while(1){
			if(toArduino.changed.load(std::memory_order_relaxed)){
#ifdef RASP_DEBUG
				std::cout<<"nh"<<std::endl;
#endif
				toArduino.mtx.lock();
				toArduino.changed.store(false);//, std::memory_order_relaxed);
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
			getFrontDistance();
			getBackDistance();
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
