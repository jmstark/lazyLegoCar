#pragma once
#ifndef SOCKET_THREAD
#define SOCKET_THREAD

#include "stdafx.h"
#include "communication.h"

void socketThreadEntry(rasp_sock::RaspberrySocket*, comSync*);

#endif