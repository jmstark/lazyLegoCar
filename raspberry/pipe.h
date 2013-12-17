#pragma once
#ifndef PIPE
#define PIPE

#ifndef PIPE_DESC
#define PIPE_DESC "/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_7413633303335150E0E1-if00"
#endif

#include "stdafx.h"

//namespace pipe{

	class USBPipe{
		public:
			USBPipe(const std::string *pipe = NULL);
			~USBPipe();
			void usbRead(uint8_t*,size_t);
			int usbWrite(void*, int);
			void usbFlush();
			inline bool connected(){return linked;}
			inline void disconnect(){if(linked)close(fd);}
			
		private:
			int fd;
			bool linked;
	};
	
//};
#endif
