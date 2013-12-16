#include "pipe.h"

using namespace std;
//using namespace pipe;

int run;

void interrupted(int signal){
	fprintf(stderr, "interrupted %d\n", signal);
	run = 0;
}

int main(int argc, char **argv){
	uint8_t buffer[32];
	uint8_t input;
	std::string name(PIPE_DESC);
	USBPipe pipe(&name);
	printf("Connection State: %s\n", pipe.connected()?"True":"False");
	if(!pipe.connected())
		return 0;
	run = 1;
	(void)signal(SIGINT, interrupted);
	while(run){
		cin>>input;
		pipe.usbWrite(&input,sizeof(input));
		pipe.usbRead(buffer,sizeof(buffer));
		std::cout << buffer << std::endl;
	}
	printf("disconnecting the service..\n");
	pipe.disconnect();
	printf("shutdown complete\n");
	return 0;
}
