#pragma once
#ifndef STDAFX
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
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <atomic>
#include <mutex>
#include <thread>
#include <math.h>

#include "../arduino/serial_cmd.h"

#ifndef ZeroMemory
#define ZeroMemory(a,b) memset(a, 0, b)
#endif
#ifndef PI
#define PI 3.14159265
#endif
#ifndef RAD
#define RAD(a) a*PI/180.0
#endif 
