#pragma once
#ifndef OBSERVER
#define OBSERVER

#include "communication.h"

	class Observer{
		public:
			Observer(rasp_sock::RaspberrySocket *sock = NULL, USBPipe *pipe = NULL);
			~Observer();
			comSync /*fromArduino,*/ toArduino;
			void run();
			void drive(uint8_t);
			void setSpeed(uint8_t);
			void steer(uint8_t);
			void moveBackLaser(uint8_t);
			void moveFrontLaser(uint8_t);
			void getFrontDistance(uint8_t *ptr=NULL);
			void controlYellowLed(bool on);
			void controlRedLed(bool on);
			uint8_t getBackDistance();
			
		private:
			int16_t angle=90;
			int8_t incrementor=1;
			rasp_sock::RaspberrySocket *sock;
			USBPipe *pipe;

	};

#endif
