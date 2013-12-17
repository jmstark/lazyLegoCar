#pragma once
#ifndef RASP_SOCK
#define RASP_SOCK
#ifndef LEGO_STD_PORT
#define LEGO_STD_PORT 14130
#endif
#ifndef BUF_LEN
#define BUF_LEN 512
#endif
#include "stdafx.h"

namespace rasp_sock{
	class RaspberrySocket{
		public:
			RaspberrySocket();
			~RaspberrySocket();
			bool initSocket();
			bool waitForClient();
			std::string receive();
			int sendMsg(const std::string&);
			void cleanup();
			
		private:
			int s_socket, c_socket;
			struct sockaddr_in server, client;
	};
};

#endif