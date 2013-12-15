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
#define LASERDATA_LEN 6

typedef struct communicationDataset{
	int speed;							//Speed Data, between 0 and 255
	int direction;						//backward -1, stop 0, forward 1
	int steering;						//left -1, straight 0, right 1
	float laserData[LASERDATA_LEN];		//data of the 6 lasers
} com;

typedef com *comPtr;


typedef struct communicationSync{
	com com;
	std::atomic<bool> changed;
/*
use block to prevent the update threads from writing 
--> pathfinding algo must have higher privileges than other writing threads to prevent collisions
*/
	std::atomic<bool> block;
	std::mutex mtx;	
} comSync;


std::string createJSON(comPtr ptr){
	char buffer[10];
	std::stringstream json;
	if(ptr == NULL)
		return "";
	//speed
	json << "{" << JSON_SPEED << ":" << ptr->speed << ",";
	//direction
	json << JSON_DIR << ":" << ptr->direction << ",";
	//steering
	json << JSON_STEERING << ":" << ptr->steering << ",";
	//laserdata
	json << JSON_LASERDATA << ":[";
	for(int i = 0; i < LASERDATA_LEN; i++){
		json << ptr->laserData[i];
		if(i < LASERDATA_LEN - 1)
			json << ",";
	}
	json << "]}";
	return json.str();
}

//returns 0 on success
int setComValue(const std::string &str, comPtr ptr){
	char key[32], val[32];
	sscanf(str.c_str(), "%s=%s", key, val);
	if(strcmp(key, JSON_SPEED) == NULL){
		ptr->speed = atoi(val, 10);
		return 0;
	}
	else if(strcmp(key, JSON_DIR) == NULL){
		ptr->direction = atoi(val, 10);
		return 0;
	}
	else if(strcmp(key, JSON_STEERING) == NULL){
		ptr->steering = atoi(val, 10);
		return 0;
	}
	//laserdata mustn't be set
	return 1;
}

#endif
