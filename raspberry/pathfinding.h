#ifndef PATHFINDING
#define PATHFINDING

#include "stdafx.h"
#include "communication.h"
#include "observer.h"

#define WENDEKREISRADIUS 56
#define SPEED 0.016667//16.67
#define RADSPEED 0.017084

/*
Breite: 			22cm

Radius
Innen: 				34cm
Außen: 				56cm


Geschwindigkeit: 	0.16667 m/s --> 0.016667 cm/ms
Winkelgeschw.:		17.084 deg/s --> 0.017084 deg/ms
*/
typedef struct point {double x,y;}pt;
typedef struct function{
	double t;
	pt m;
	double f(double x){return m.y*x/m.x+t;}
	double getM(){return m.y/m.x;}
} func;
typedef struct _direction{
	uint8_t drv_info;
	clock_t t;
} Direction;



class Path{
	public:
		Path(double x, double y, comSync *data, Observer *obs);
		~Path(){}
		void calculatePath();
		void drive();
		
	private:
		double /*x, y,*/ dist, rad, radToDest, midRad;
		function f;
		pt dst, mid, pos;
		time_t last;
		comSync *data;
		bool circle;
		std::queue<Direction> dir;
		Observer *obs;
		void calcNewPos(clock_t,int flag = 1);
		int comparePos(){return abs(pos.x-dst.x) < 1.0 && abs(pos.y-dst.y) < 1.0 ? 0:1;}
		void parallelToObstacle();
		void driveCar(uint8_t drv);
		void setDirection(uint8_t dir);
		void computeMiddle();
};

#endif