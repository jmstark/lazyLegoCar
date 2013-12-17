#include "communication.h"

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
		json << ptr->laserDataFront[i];
		json << ",";
	}
	json<<ptr->laserDataBack;
	json << "]}";
	return json.str();
}

//returns 0 on success
int setComValue(const std::string &str, comPtr ptr){
	char key[32], val[32];
	sscanf(str.c_str(), "%s=%s", key, val);
	if(strcmp(key, JSON_SPEED) == NULL){
		ptr->speed = atoi(val);
		return 0;
	}
	else if(strcmp(key, JSON_DIR) == NULL){
		ptr->direction = atoi(val);
		return 0;
	}
	else if(strcmp(key, JSON_STEERING) == NULL){
		ptr->steering = atoi(val);
		return 0;
	}
	//laserdata mustn't be set
	return 1;
}


