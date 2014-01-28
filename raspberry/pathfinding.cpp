#include "pathfinding.h"

using namespace std;

Path::Path(double x, double y, comSync *data, Observer *obs){
	this->dst.x = x;
	this->dst.y = y;
	this->obs = obs;
	dist = hypot(x,y);
	rad = atan2(x,y);
	mid = {WENDEKREISRADIUS,0};
	this->data = data;
	pos.x = 0;
	pos.y = 0;
	circle = false;
	f.m = {x,y};
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
}

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
*/

void Path::calculatePath(){
	//double rad;
	Direction d;
	pt P;

	ZeroMemory(&d, sizeof(Direction));
	//rad = RAD(midRad);
	auto calcF = [&]()->bool{
		P.y = mid.y + WENDEKREISRADIUS * sin(rad);
		f.m = { P.x - mid.x, mid.y - P.y };
		f.t = P.y - P.x * f.m.y / f.m.x;
		int y = f.f(dst.x);
#ifdef RASP_DEBUG
		printf("%d\n", y);
#endif
		return dst.y - 10 <= y && dst.y + 10 >= y;
	};
	auto pushWP = [&](double way, int steering)->void{
		d.drv_info = steering;
#ifdef RASP_DEBUG
		printf("drv_info: %d\tsteering: %d\n", d.drv_info, steering);
#endif
		if (steering == 0)
			d.t = way / SPEED;
		else if (steering == -1 || steering == 1)
			d.t = way / RADSPEED;
		dir.push(d);
	};
	
	if (dst.x < 0){	//-->left
		for (int angle = 0; angle < 360; angle++){
			rad = cos(RAD(angle));
			P.x = mid.x + WENDEKREISRADIUS * rad;
			if (calcF()){
				pushWP(angle, -1);
				break;
			}
		}
	}
	else{						//-->right
		for (int angle = 0; angle < 360; angle++){
			rad = cos(RAD(angle));
			P.x = mid.x - WENDEKREISRADIUS * rad;
			if (calcF()){
				pushWP(angle, 1);
				break;
			}
		}
	}
	//now just need to drive straight to the dst
	pushWP(hypot(dst.x - P.x, dst.y - P.y), 0);
}


void Path::calcNewPos(clock_t t){
	//double way;
	double rad;
	if(data->comc.steering == -1 || data->comc.steering == 1){	
		midRad += RADSPEED*t;
		rad = RAD(midRad);
		if(data->comc.steering == -1)	//-->left
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
	clock_t start, stp;
	Direction *d;
	auto driveCar = [&](uint8_t drv)->void{
		data->mtx.lock();
		data->changed.exchange(true);
		data->comc.direction = drv;
		data->mtx.unlock();
	};
#ifdef RASP_DEBUG
	cout << "starting pathfinding algorithm" << endl;
	cout << "calculating path" << endl;
#endif
	driveCar(0);
	calculatePath();
#ifdef RASP_DEBUG
	cout << "calculation completed" << endl;
#endif
	while(!dir.empty()){
		d = &dir.front();
		dir.pop();
		stp = 0;
#ifdef RASP_DEBUG
		printf("driving %l ms in direction %d\nsetting direction", d->t,d->drv_info);
#endif
		data->mtx.lock();
		data->changed.store(true);
		data->comc.steering = d->drv_info;
		data->mtx.unlock();
#ifdef RASP_DEBUG
		printf("waiting for 2s");
#endif
		sleep(2);
#ifdef RASP_DEBUG
		printf("start driving\n");
#endif
		driveCar(1);
		start = clock()/(CLOCKS_PER_SEC/1000);
		while(stp < d->t){
			if(data->comc.laserDataFront[2] > 23 || data->comc.laserDataFront[2] < 60){
				driveCar(0);
				printf("obstacle ahead\n");
				return;
			}
			stp = (clock()/(CLOCKS_PER_SEC/1000))-start;
		}
		driveCar(0);
		calcNewPos(d->t);
#ifdef RASP_DEBUG
		printf("new position: P(%d|%d)\n", pos.x, pos.y);
#endif
		sleep(1);
	}
#ifdef RASP_DEBUG
		printf("reached destination, stopping vehicle\n");
#endif
	//stop
	data->mtx.lock();
	data->changed.exchange(true);
	data->comc.direction = 0;
	data->mtx.unlock();
#ifdef RASP_DEBUG
		printf("END\n");
#endif
}
