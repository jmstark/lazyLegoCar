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
	sleep(2);
	pipe.usbFlush();
		
	uint32_t temp;
	while(run){
		cin>>temp;
		input=static_cast<uint8_t>(temp);
		//input-='0';
		pipe.usbFlush();
		pipe.usbWrite(&input,sizeof(input));
		//sleep(1);
		//pipe.usbRead(buffer,sizeof(buffer));
		//for(int i=0;i<sizeof(buffer);i++)
		//std::cout << static_cast<int>(buffer[i]) << std::endl;
	}
	printf("disconnecting the service..\n");
	pipe.disconnect();
	printf("shutdown complete\n");
	return 0;
}
