#pragma once
#ifndef STADFX
#define STDAFX

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>


#ifndef ZeroMemory
#define ZeroMemory(a,b) memset(a, 0, b)
#endif

#endif 
