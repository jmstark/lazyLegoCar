#pragma once
#ifndef PIPE
#define PIPE "/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_7413633303335150E0E1-if00"

#include "stdafx.h"

class USBPipe{
	public:
		USBPipe(const std::string *pipe = NULL);
		~USBPipe();
		std::string usbRead();
		inline bool connected(){return linked;}
		inline void disconnect(){if(linked)close(fd);}
		
	private:
		int fd;
		bool linked;
};

#endif