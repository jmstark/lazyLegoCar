#include "socket.h"

using namespace std;
using namespace rasp_sock;

int main(void){
	cout << "starting socket" << endl;
	RaspberrySocket s;
	s = RaspberrySocket();
	if(!s.initSocket()){
		cout << "failed open socket" << endl;
		return 0;
	}
	cout << "initialized socket, waiting for client to connect..." << endl;
	if(!s.waitForClient()){
		cout << "failed to accept client" << endl;
		return 0;
	}
	cout << "client connected" << endl;
	string rd;
	while(1){
		rd = s.receive();
		if(rd.compare("") == 0){
			cout << "the client disconnected from the server, shutting down" << endl;
			break;
		}
		cout << rd << endl;
		if(s.sendMsg(rd) < 0){
			cout << "the client disconnected from the server, shutting down" << endl;
			break;
		}
	}
	return 0;
}