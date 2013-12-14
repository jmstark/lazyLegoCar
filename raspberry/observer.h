#pragma once
#ifndef OBSERVER
#define OBSERVER

#include "communication.h"

namespace obs{

	class Observer{
		public:
			Observer(rasp_sock::RaspberrySocket *sock = NULL, pipe::USBPipe *pipe = NULL);
			~Observer();
			comSync fromArduino, toArduino;
			void run();
			
		private:
			rasp_sock::RaspberrySocket *sock;
			pipe::USBPipe *pipe;
	};
};
#endif