#ifndef PATHFINDING
#define PATHFINDING

#include "stdafx.h"
#define WENDEKREISRADIUS 30

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