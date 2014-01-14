#ifndef PATHFINDING
#define PATHFINDING

#include "stdafx.h"
#define WENDEKREISRADIUS 56
/*
Breite: 			22cm

Radius
Innen: 				34cm
Auﬂen: 				56cm


Geschwindigkeit: 	0.16667 m/s
Winkelgeschw.:		0.188 rad/s
*/
typedef struct point {double x,y;}pt;

class Path{
	public:
		Path(double x, double y);
		~Path(){}
		void calculatePath();
		
	private:
		double x, y, dist, rad, radToDest;
};

#endif