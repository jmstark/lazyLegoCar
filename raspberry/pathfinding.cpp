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


void Path::calcNewPos(clock_t t, int flag){
	//double way;
	double rad;
	auto calcF = [&]()->bool{
		pos.y = mid.y + WENDEKREISRADIUS * sin(rad);
		f.m = { pos.x - mid.x, mid.y - pos.y };
		f.t = pos.y - pos.x * f.m.y / f.m.x;
		int y = f.f(dst.x);
#ifdef RASP_DEBUG
		printf("%d\n", y);
#endif
		return dst.y - 5 <= y && dst.y + 5 >= y;
	};
	
	if(data->comc.steering == -1 || data->comc.steering == 1){	
		midRad += RADSPEED*t;
		rad = RAD(midRad);
#ifdef RASP_DEBUG
		printf("midRad: %g,(deg), %g(rad)\n", midRad, rad);
#endif 
		rad = RAD(midRad);
		if(data->comc.steering == -1)	//-->left
			pos.x = mid.x + WENDEKREISRADIUS * cos(rad);
		else						//-->right
			pos.x = mid.x - WENDEKREISRADIUS * cos(rad);
		calcF();
		/*
		pos.y = mid.y + WENDEKREISRADIUS * sin(rad);
		f.m = {pos.x-mid.x, mid.y-pos.y};
		f.t = pos.y - pos.y * f.m.y / f.m.x;
		*/
	}
	else{
		//here rad == way
		rad = t * SPEED;
		double alpha = atan(pos.x/pos.y);
		double hypotenuse;
		if(flag)//data->comc.direction == 1 || data->comc.direction == 0)
			hypotenuse = rad + hypot(pos.x,pos.y);
		else
			hypotenuse = rad - hypot(pos.x,pos.y);
		pos.x = sin(alpha)*hypotenuse;
		pos.y = cos(alpha)*hypotenuse;
	}
}


void Path::parallelToObstacle(){
	uint64_t ms = (2*WENDEKREISRADIUS-data->comc.laserDataFront[2])/SPEED;
	clock_t t, start;
	start = clock()/(CLOCKS_PER_SEC/1000);
	t = 0;
	driveCar(-1);
	calcNewPos(ms, 0);
	while(t-start < ms) t = clock()/(CLOCKS_PER_SEC/1000);
	driveCar(0);
	ms = 90/RADSPEED;
	setDirection(1);
	calcNewPos(ms);
	computeMiddle();
	start = clock()/(CLOCKS_PER_SEC/1000);
	driveCar(1);
	t = 0;
	while(t-start < ms) t = clock()/(CLOCKS_PER_SEC/1000);
	driveCar(0);
}

void Path::driveCar(uint8_t drv){
	data->mtx.lock();
	data->changed.exchange(true);
	data->comc.direction = drv;
	data->mtx.unlock();
}


void Path::setDirection(uint8_t dir){
	data->mtx.lock();
	data->changed.store(true);
	data->comc.steering = dir;
	data->mtx.unlock();
}
void Path::computeMiddle()
{
	//double alpha=tan(f.getM());
	//double beta=180-90-alpha;
	//double gamma=90-beta;
	double gamma=tan(f.getM());
	//double a=cos(gamma)*WENDEKREISRADIUS;
	//double b=sin(gamma)*WENDEKREISRADIUS;
	mid.y=pos.y-cos(gamma)*WENDEKREISRADIUS;
	mid.x=pos.x+data->comc.steering*sin(gamma)*WENDEKREISRADIUS;
	
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
		midRad = 0;
#ifdef RASP_DEBUG
		printf("driving %ld ms in direction %hhd\nsetting direction\n", d->t,d->drv_info);
#endif
		setDirection(d->drv_info);
#ifdef RASP_DEBUG
		printf("waiting for 2s\n");
#endif
		sleep(2);
#ifdef RASP_DEBUG
		printf("start driving\n");
		//printf("Start: P(%g|%g)\n", pos.x, pos.y);
#endif
		driveCar(1);
		start = clock()/(CLOCKS_PER_SEC/1000);
		while(stp < d->t){
			//printf("%hhu, %hhu, %hhu, %hhu, %hhu\n",data->comc.laserDataFront[0],data->comc.laserDataFront[1],data->comc.laserDataFront[2],data->comc.laserDataFront[3],data->comc.laserDataFront[4]);
			if(data->comc.laserDataFront[2] > 23 && data->comc.laserDataFront[2] < 60 && data->comc.steering == 0){
				//obstacle in front and driving straight
				driveCar(0);
				calcNewPos(stp);
				printf("obstacle ahead\n");
				parallelToObstacle();
				return;
			}
			stp = (clock()/(CLOCKS_PER_SEC/1000))-start;
		}
		driveCar(0);
		calcNewPos(d->t);
#ifdef RASP_DEBUG
		printf("new position: P(%g|%g)\n", pos.x, pos.y);
#endif
		sleep(1);
	}
#ifdef RASP_DEBUG
		printf("reached destination, stopping vehicle\n");
#endif
	//stop
	driveCar(0);
#ifdef RASP_DEBUG
		printf("END\n");
#endif
}
