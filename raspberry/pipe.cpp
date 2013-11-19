#include "pipe.h"


USBPipe::USBPipe(const std::string *pipe){
	if(pipe == NULL){
		this->linked = false;
		return;
	}
	this->fd = open (pipe->c_str(), O_RDWR | O_NOCTTY | O_SYNC);
	if(this->fd < 0){
		fprintf(stderr,"error %d opening %s %s\n", errno, pipe->c_str(), strerror(errno));
		this->linked = false;
	}
	else{
		
		this->linked = true;
	}
}


USBPipe::~USBPipe(){
	if(linked){
		close(fd);
		linked = false;
	}
}


std::string USBPipe::usbRead(){
	char buffer[512];
	int rd;
	if(!linked)
		return "";
	memset(buffer, 0, 512);
	rd = read(fd, buffer, 512);
	if(rd == 0)
		return "";
	std::string result(buffer);
	return result;
}

int run;

void interrupted(int signal){
	fprintf(stderr, "interrupted %d\n", signal);
	run = 0;
}

int main(int argc, char **argv){
	std::string name(PIPE);
	USBPipe pipe(&name);
	printf("Connection State: %s\n", pipe.connected()?"True":"False");
	if(!pipe.connected())
		return 0;
	run = 1;
	(void)signal(SIGINT, interrupted);
	while(run){
		std::cout << pipe.usbRead() << std::endl;
	}
	printf("disconnecting the service..\n");
	pipe.disconnect();
	printf("shutdown complete\n");
	return 0;
}