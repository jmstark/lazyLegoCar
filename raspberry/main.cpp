#include "observer.h"
#include <unistd.h>
#include "threads.h"

int main()
{
	uint8_t fd[5];
	std::string ps=PIPE_DESC;
	USBPipe p(&ps);
	rasp_sock::RaspberrySocket s;
	s.initSocket();
	
	Observer o(&s,&p);
	std::thread t1(socketThreadEntry,&s,&o.toArduino, &o);
	//std::thread t2(pathFindingThread, &o);
	//o.run();
	sleep(10000);
	/*sleep(2);
	p.usbFlush();
	sleep(2);
	o.drive(DIR_FWD);
	sleep(1);
	o.steer(LEFT);	
	sleep(1);
	std::cout<<static_cast<int>(o.getBackDistance());
	sleep(1);
	o.getFrontDistance(fd);
	for(int i=0;i<5;i++)
		std::cout<<static_cast<int>(fd[i])<<' ';
	sleep(1);
	o.moveFrontLaser(70);
	sleep(1);
	o.moveFrontLaser(30);

	o.moveBackLaser(30);
	sleep(3);*/
	
	return 0;	
}
