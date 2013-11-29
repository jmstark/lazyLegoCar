#include <Servo.h>
#include <SharpIR.h>

//pins and constants
#define LASER_STEERING 12
#define MOTORS_I 7    //Digital-Steering
#define MOTORS_II 8   //Digital-Steering
#define MOTOR_I 2    //Propulsion FrontDir
#define MOTOR_II 3  //Propulsion BackDir
#define SPEED_MOTOR_A 9//H-Bridge
#define LASER_SENSOR_R A0  //Analog
#define LASER_SENSOR_B A1 //Analog
#define DIR_STOP 0
#define DIR_FWD 1
#define DIR_RWD 2
#define STRAIGHT 0
#define LEFT 1
#define RIGHT 2

//enables some output useful for debugging
#define DEBUG 
//defines how many steps the sensor is moving
#define RANGE_IN_STEPS 5
//defines the size of a step
#define STEP_DEGREES 10
//defines, where the sensor starts moving, so that it always covers the area in front of the car. 
//do not change, as this should always work
#define STEP_OFFSET (90-((RANGE_IN_STEPS-1)*STEP_DEGREES)/2)
//loop delay in milliseconds. this determines, how much time will be waited after a run of the loop() method.
//mainly the laser scanning and moving is affected by this. Do not set this value too small, 
//as the laser servo needs some time to move (depending on STEP_DEGREES) and the laser may return 
//wrong values if it gets polled too quickly. 170 should be a reasonable choice.
#define LOOP_DELAY 170
//see loop() for the following two constants.
#define DISTANCE_FAR 75
#define DISTANCE_NEAR 23




//if the sensor should cover the whole 180 degrees in front of it, this defines how many steps they are divided into.
//currently not in use, maybe re-used in future versions
#define NUM_STEPS (180/STEP_DEGREES)
//if the sensor should cover the whole 180 degrees in front of it,
//this defines which sensor values (or how much of them) will trigger collision avoidance.
//currently not in use, maybe re-used in future versions
#define CRIT_RANGE 5
//middlemost sensor data
//currently not in use, maybe re-used in future versions
#define FRONT_VAL (NUM_STEPS/2)


//arrays for storing laser data
int frontDist[RANGE_IN_STEPS];
int backDist[RANGE_IN_STEPS];

bool isSteering=false;
bool obstacle=false;
bool stop=false;

byte stepCnt=0;

SharpIR backIR = SharpIR(GP2Y0A02YK,0);
SharpIR frontIR = SharpIR(GP2Y0A02YK,1);
Servo s;

//incrementor for stepCnt. See laser().
int inc=-1;//gets inverted on first run of laser()

void drive(int dir);

void setup()                    // run once, when the sketch starts
{
  //Basic setup
  Serial.begin(9600);// set up Serial library at 9600 bps
  s.attach(LASER_STEERING);
  pinMode(MOTOR_I, OUTPUT);
  pinMode(MOTOR_II, OUTPUT);
  pinMode(SPEED_MOTOR_A, OUTPUT);
  digitalWrite(SPEED_MOTOR_A, HIGH); 

  //initialize the arrays with big values, so that no obstacle is assumed at the beginning.
  for(int i=0;i<RANGE_IN_STEPS;i++)
  {
    frontDist[i]=255;
    backDist[i]=255;
  }

  //Get laser into starting position
  s.write(STEP_OFFSET + stepCnt*STEP_DEGREES);

  Serial.println("Program started.");
  //testing the steering
  delay(1000);
  steer(RIGHT);
  delay(1000);
  steer(STRAIGHT);
  delay(1000);
  steer(LEFT);
  delay(1000);
  steer(STRAIGHT);
  delay(1000);

  //begin driving. loop() takes care of obstacles.
  drive(DIR_FWD);

}


void laser()
{
  //store the values to avoid multiple sequential sensor readings which may produce wrong results
  int front_Dist=frontIR.getData();
  int back_Dist=backIR.getData();

  //The laser only works unto a certain distance. values above this distance are set to a reasonable value.
  frontDist[stepCnt]=front_Dist>80?90:front_Dist;
  backDist[stepCnt]=back_Dist>80?90:back_Dist;

  //If the laser has reached the left or right corner, simply negate the incrementor
  //so that the laser turns into the opposite direction next time.
  //Also, print out the current values.
  if(stepCnt == RANGE_IN_STEPS -1 | stepCnt==0)
  {
    inc=-inc;
    
    #ifdef DEBUG
    Serial.print(stepCnt);
    Serial.print("\t\t");
    for(byte i=0;i<RANGE_IN_STEPS;i++)
      {
        Serial.print(frontDist[i]);
        Serial.print('\t');
      }
    Serial.println();
    #endif
  }

  stepCnt+=inc;
  s.write(STEP_OFFSET + stepCnt*STEP_DEGREES);
}

//set the driving direction. possible values: DIR_STOP, DIR_FWD, DIR_RWD
void drive(int dir){
  if(dir == DIR_STOP){
    digitalWrite(MOTOR_I, LOW);
    digitalWrite(MOTOR_II, LOW);
  }
  else if(dir == DIR_FWD){
    digitalWrite(MOTOR_I, HIGH);
    digitalWrite(MOTOR_II, LOW);
  }
  else if(dir == DIR_RWD){
    digitalWrite(MOTOR_I, LOW);
    digitalWrite(MOTOR_II, HIGH);
  }
}

//control steering. possible values: STRAIGHT, LEFT, RIGHT
void steer(byte dir){
 switch(dir)
  {
  case STRAIGHT:
     digitalWrite(MOTORS_I, LOW);
     digitalWrite(MOTORS_II, LOW);
     break;
   case RIGHT:
     digitalWrite(MOTORS_I, LOW);
     digitalWrite(MOTORS_II, HIGH);
     break;
   case LEFT:
     digitalWrite(MOTORS_I, HIGH);
     digitalWrite(MOTORS_II, LOW);
     break;
   default: break;
   } 
}


//steers the car into the direction in which the sum of all sensor values is bigger.
//->probably less obstacles
void steer()
{
  int sumLeft=0,sumRight=0;
  for(int i=0;i<RANGE_IN_STEPS/2;i++)
  {
    sumLeft+=frontDist[i];
    sumRight+=frontDist[i+RANGE_IN_STEPS/2+RANGE_IN_STEPS%2];
  }

  if(sumLeft<=sumRight)
  {
    steer(LEFT);    
  }
  else
  {
    steer(RIGHT);    
  }
  
}


//main loop
void loop()                       // run over and over again
{
  laser();

  //If a sensor value smaller than DISTANCE_FAR is found, call steer() to avoid the obstacle.
  //If a sensor value smaller than DISTANCE_NEAR is found, stop the car.
  obstacle=false;
  stop=false;
  for(int i=0;i<RANGE_IN_STEPS;i++)
  {
    if(frontDist[i]<DISTANCE_FAR)
    {
      obstacle=true;
      if(!isSteering)
      {
        isSteering=true;
        steer();
      }
    }
    if(frontDist[i]<DISTANCE_NEAR)
    {
      stop=true;
      drive(DIR_STOP);
    }
  }
  if(!obstacle)
  {  
    if(isSteering)
    {
      steer(STRAIGHT);
      isSteering=false;
    }
  }

  delay(LOOP_DELAY); 
}
