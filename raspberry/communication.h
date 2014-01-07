#pragma once
#ifndef COMMUNICATION
#define COMMUNICATION

#include "stdafx.h"
#include "pipe.h"
#include "socket.h"

#define JSON_SPEED "Speed"
#define JSON_DIR "Direction"
#define JSON_STEERING "Steering"
#define JSON_LASERDATA "LaserData"
#define LASERDATA_LEN 5

#define COMMAND_FWD "Forwards\n"
#define COMMAND_RWD "Backwards\n"
#define COMMAND_LEFT "Left\n"
#define COMMAND_RIGHT "Right\n"
#define COMMAND_STOP "Stop\n"
#define COMMAND_STRAIGHT "Straight\n"

typedef struct communicationDataset{
	int speed;							//Speed Data, between 0 and 255
	int direction;						//backward -1, stop 0, forward 1
	int steering;						//left -1, straight 0, right 1
	uint8_t laserDataFront[LASERDATA_LEN];
	uint8_t laserDataBack;
} com;

typedef com *comPtr;


typedef struct communicationSync{
	com comc;
	std::atomic<bool> changed;
/*
use block to prevent the update threads from writing 
--> pathfinding algo must have higher privileges than other writing threads to prevent collisions
*/
	std::atomic<bool> block;
	std::mutex mtx;	
} comSync;


std::string createJSON(comPtr ptr);

//returns 0 on success
int setComValue(const std::string &str, comPtr ptr);

#endif
