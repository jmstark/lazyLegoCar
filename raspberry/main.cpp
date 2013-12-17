#include "observer.h"
#include <unistd.h>

int main()
{
	uint8_t fd[5];
	std::string s=PIPE_DESC;
	USBPipe p(&s);
	rasp_sock::RaspberrySocket s;
	s.initSocket();
	
	Observer o(&s,&p);
	std::thread t(socketThreadEntry,&s,&o.toArduino);
/*	sleep(2);
	p.usbFlush();
	sleep(2);
	o.drive(DIR_FWD);
	sleep(1);
	o.steer(LEFT);	
	sleep(1);
	std::cout<<static_cast<int>(o.getBackDistance());
	sleep(1);
	o.getFrontDistance(fd);
	for(int k=0;k<99;k++);
	for(int i=0;i<5;i++)
		std::cout<<static_cast<int>(fd[i])<<' ';
	sleep(1);
	o.moveFrontLaser(90);
	sleep(1);

	o.moveBackLaser(45);*/
	
	return 0;	
}
