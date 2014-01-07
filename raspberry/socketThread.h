#pragma once
#ifndef SOCKET_THREAD
#define SOCKET_THREAD

#include "stdafx.h"
#include "communication.h"
#include "observer.h"

void socketThreadEntry(rasp_sock::RaspberrySocket*, comSync*, Observer*);

#endif
