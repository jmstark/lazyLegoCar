#include "pathfinding.h"

using namespace std;

Path::Path(double x, double y, comSync *data){
	this->dst.x = x;
	this->dst.y = y;
	dist = hypot(x,y);
	rad = atan2(x,y);
	mid = {0, WENDEKREISRADIUS};
	this->data = data;
	pos.x = 0;
	pos.y = 0;
	circle = false;
	f.m = {0,1};
	f.t = 0;
	last = 0;
	midRad = 0;
}

/*
void Path::calculatePath(){
	if(rad == 0){
		radToDest = 0;
		return;
	}
	pt P, m;	//steigung -> m, Punkt auf Kreis -> P
	double crRad, radY, t;
	for(crRad = 1; crRad < 360.0; crRad++){
		//calculate x/y on the circle
		P.x = std::cos(RAD(crRad))*WENDEKREISRADIUS+mid.x;
		P.y = std::sin(RAD(crRad))*WENDEKREISRADIUS+mid.y;
		
		//calculate gradient mid to P
		m = {mid.x-P.x, mid.y-P.y};
		
		//calculate verticle equation 
		t = P.y + m.x * P.x / m.y;
		
		//calculate the y pos with dst x coord
		t = m.x * this->x / m.y + t;
		if(this->y-0.5 <=  t || this->y+0.5 >= t){
			radToDest == RAD(crRad);
			break;
		}
	}
}*/

int Path::calculatePath(){
	time_t current = time(NULL);
	time_t diff = current-last;
	double result;
	calcNewPos(diff);
	if(!circle){
		result = f.f(dst.x)
		if(result == dst.y-0.5 || result == dst.y+0.5){
			calcNewPos(diff);
			return 0;
		}
	}
	//fehlt circle
}

void Path::calcNewPos(time_t t){
	//double way;
	double rad;
	if(circle){	
		midRad += RADSPEED*t;
		rad = RAD(midRad);
		if(data->steering == -1)	//-->left
			pos.x = mid.x + WENDEKREISRADIUS * sin(rad);
		else						//-->right
			pos.x = mid.x - WENDEKREISRADIUS * sin(rad);
		pos.y = mid.y + WENDEKREISRADIUS * cos(rad);
		f.m = {pos.x-mid.x, mid.y-pos.y};
		f.t = pos.y - pos.y * f.m.y / f.m.x;
	}
	else{
		//here rad == way
		rad = t * SPEED;
		
		//test to calc new pos
		rad = rad/f.getM();
		pos.x += pos.x*rad;
		pos.y += pos.y*rad;
	}
}

void Path::drive(){
	/*
	Idee:
		1. calculate direct Path to dst
		2. check for obstacles on the direct path
			if obstacle decide wether left or rigth / find/search gap
			--> new sub path, begin with 1
			else drive to dst
	*/
	while(comparePos() != 0){
		//toDo
	}
	//stop
}
