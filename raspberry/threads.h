#pragma once
#ifndef SOCKET_THREAD
#define SOCKET_THREAD

#include "stdafx.h"
#include "communication.h"
#include "observer.h"
#include "pathfinding.h"

void socketThreadEntry(rasp_sock::RaspberrySocket*, comSync*, Observer*);
void pathFindingThread(comSync*, Observer*);
#endif
