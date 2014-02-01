#include "socket.h"

namespace rasp_sock{
	RaspberrySocket::RaspberrySocket(){
		this->s_socket = 0;
		this->c_socket = -1;
		ZeroMemory(&server, sizeof(struct sockaddr_in));
		ZeroMemory(&client, sizeof(struct sockaddr_in));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = htonl(INADDR_ANY);
		server.sin_port = htons(LEGO_STD_PORT);
	}


	RaspberrySocket::~RaspberrySocket(){
		if(s_socket != 0)
			close(s_socket);
		if(c_socket != 0)
			close(c_socket);
	}
	
	
	bool RaspberrySocket::initSocket(){
		if(this->s_socket != 0){
			return false;
		}
		this->s_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(this->s_socket < 0){
			printf("s_sock:(%d) %s\n", errno, strerror(errno));
			s_socket = 0;
			return false;
		}
		if(bind(this->s_socket, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) < 0){
			printf("s_sock:(%d) %s\n", errno, strerror(errno));
			close(this->s_socket);
			this->s_socket = 0;
			return false;
		}
		if(listen(this->s_socket, 5) < 0){
			printf("s_sock:(%d) %s\n", errno, strerror(errno));
			close(this->s_socket);
			this->s_socket = 0;
			return false;
		}
		return true;
	}
	
	
	bool RaspberrySocket::waitForClient(){
		socklen_t length;
		if(c_socket != 0)
			return true;
		length = sizeof(struct sockaddr_in);
		this->c_socket = accept(s_socket, (struct sockaddr*)&client, &length);
		if(c_socket < 0){
			c_socket = -1;
			printf("c_sock:(%d) %s\n", errno, strerror(errno));
			return false;
		}
		else
			return true;
	}
	
	
	std::string RaspberrySocket::receive(){
		if(c_socket == 0)
			return "";
		char buffer[BUF_LEN] = {0};
		std::string result = "";
		int read = BUF_LEN;
		while(read >= BUF_LEN){
			ZeroMemory(buffer, BUF_LEN);
			read = recv(c_socket, buffer, BUF_LEN, 0);
			result += buffer;
		}
		if(read < 0){
			cleanup();
			return "";
		}
		return result;
	}
	
	
	int RaspberrySocket::sendMsg(const std::string& toSend){
		int result;
		if(c_socket == 0)
			return -1;
		result = send(this->c_socket, toSend.c_str(), toSend.size(), 0);
		return result;
	}
	
	
	void RaspberrySocket::cleanup(bool flag){
		if(s_socket != 0 && flag){
			close(s_socket);
			s_socket = 0;
		}
		if(c_socket != 0){
			close(c_socket);
			c_socket = 0;
			ZeroMemory(&client, sizeof(struct sockaddr_in));
		}
	}
};