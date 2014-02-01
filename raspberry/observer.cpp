#include "observer.h"

using namespace std;


	Observer::Observer(rasp_sock::RaspberrySocket *sock,USBPipe *pipe){
		this->sock = sock;
		this->pipe = pipe;
		toArduino.x = 0;
		toArduino.y = 0;
		toArduino.pathFinding = false;
	}
	
	
	Observer::~Observer(){
	
	}

	void Observer::controlYellowLed(bool on)
	{
#ifdef RASP_DEBUG
		printf("Yellow LED\n");
#endif
		pipe->usbWrite(on?(void*)&CMD_YELLOW_LED_ON:(void*)&CMD_YELLOW_LED_OFF,sizeof(uint8_t));
	}

        void Observer::controlRedLed(bool on)
        {
#ifdef RASP_DEBUG
                printf("Red LED\n");
#endif
                pipe->usbWrite((on?((void*)&CMD_RED_LED_ON):((void*)&CMD_RED_LED_OFF)),sizeof(uint8_t));
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

		uint8_t mult = (direction==LEFT)?(-1):((direction==RIGHT)?0:1);
		moveFrontLaser(90-30*mult);
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
#ifdef RASP_DEBUGG
		cout << "reading front dist" << endl;
#endif
		pipe->usbFlush();
		pipe->usbWrite((void*)&CMD_GET_LASERDATA_FRONT,sizeof(uint8_t));
		pipe->usbRead(toArduino.comc.laserDataFront,LASERDATA_LEN,LASERDATA_LEN);
		if (dist != NULL){
			for(int i=0;i<LASERDATA_LEN;i++)
			{
				dist[i]=toArduino.comc.laserDataFront[i];
			}	
		}
	}

	uint8_t Observer::getBackDistance()
	{
#ifdef RASP_DEBUGG
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

			usleep(150*1000);
			angle+=incrementor*10;
			if(angle>=180 || angle <= 0)
				incrementor=-incrementor;
			

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
