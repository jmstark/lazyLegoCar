#ifndef PATHFINDING
#define PATHFINDING

#include "stdafx.h"
#include "communication.h"

#define WENDEKREISRADIUS 56
#define SPEED 16.67
#define RADSPEED 17.084
/*
Breite: 			22cm

Radius
Innen: 				34cm
Auﬂen: 				56cm


Geschwindigkeit: 	0.16667 m/s
Winkelgeschw.:		17.084 deg/s
*/
typedef struct point {double x,y;}pt;
typedef struct function{
	double t;
	pt m;
	double f(double x){return m.y*x/m.x+t;}
	double getM(){return m.y/m.x;}
} func;

class Path{
	public:
		Path(double x, double y, comSync *data);
		~Path(){}
		int calculatePath();
		void drive();
		
	private:
		double /*x, y,*/ dist, rad, radToDest, midRad;
		function f;
		pt dst, mid, pos;
		time_t last;
		comSync *data;
		bool circle;
		
		void calcNewPos(time_t);
		int comparePos(){return abs(pos.x-dst.x) < 1.0 && abs(pos.y-dst.y) < 1.0 ? 0:1;}
};

#endif