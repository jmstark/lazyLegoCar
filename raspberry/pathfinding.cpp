#include "pathfinding.h"

using namespace std;

Path::Path(double x, double y){
	this->x = x;
	this->y = y;
	dist = hypot(x,y);
	rad = atan2(x,y);
}


void Path::calculatePath(){
	if(rad == 0){
		radToDest = 0;
		return;
	}
	pt P, m;
	pt mid = {0, WENDEKREISRADIUS};
	double crRad, radY, t;
	for(crRad = 1; crRad < 360.0; crRad++){
		//calculate x/y on the circle
		P.x = std::cos(RAD(crRad))*WENDEKREISRADIUS+mid.x;
		P.y = std::sin(RAD(crRad))*WENDEKREISRADIUS+mid.y;
		
		//calculate gradient mid to P
		m = {mid.x-P.x, mid.y-P.y};
		
		//calculate verticle equation 
		t = P.y - m.x * P.x / m.y;
		
		//calculate the y pos with dst x coord
		t = m.x * this->x / m.y + t;
		if(this->y-0.5 ==  t || this->y+0.5 == t){
			radToDest == RAD(crRad);
			break;
		}
	}
}
