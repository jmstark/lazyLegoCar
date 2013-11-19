#include "pipe.h"

using namespace std;
using namespace pipe;

int run;

void interrupted(int signal){
	fprintf(stderr, "interrupted %d\n", signal);
	run = 0;
}

int main(int argc, char **argv){
	std::string name(PIPE_DESC);
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