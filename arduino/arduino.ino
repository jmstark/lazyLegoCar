#include <Servo.h>
#include <SharpIR.h>

#define DEBUG

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
#define RANGE_IN_STEPS 5
#define STEP_DEGREES 10
#define STEP_OFFSET (90-((RANGE_IN_STEPS-1)*STEP_DEGREES)/2)
#define NUM_STEPS (180/STEP_DEGREES)
#define CRIT_RANGE 5
#define FRONT_VAL (NUM_STEPS/2)
#define LOOP_DELAY 250


byte frontDist[RANGE_IN_STEPS];
byte backDist[RANGE_IN_STEPS];
float distance2;
float distance1;
bool isSteering=false;
bool obstacle=false;

byte stepCnt=0;


Servo s;
int inc=-1;//gets inverted on first run

void drive(int dir);

void setup()                    // run once, when the sketch starts
{
  for(int i=0;i<RANGE_IN_STEPS;i++)
  {
    frontDist[i]=255;
    backDist[i]=255;
  }
  Serial.begin(9600);           // set up Serial library at 9600 bps
  s.attach(LASER_STEERING);
  //Dist.begin(A1);
  Serial.println("Hello world!");
  s.write(STEP_OFFSET + stepCnt*STEP_DEGREES);
  pinMode(MOTOR_I, OUTPUT);
  pinMode(MOTOR_II, OUTPUT);
  pinMode(SPEED_MOTOR_A, OUTPUT);
  digitalWrite(SPEED_MOTOR_A, HIGH); ///
  steer(RIGHT);
  delay(1000);
  steer(STRAIGHT);
  delay(1000);
  //drive(DIR_FWD);
  
}


void laser()
{
  frontDist[stepCnt]=getFrontDist();
  backDist[stepCnt]=getBackDist();
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

byte getFrontDist()
{
  float volts2 = analogRead(LASER_SENSOR_B)*5.0/1024.0;
  distance2 = 65.0 * pow(volts2, -1.10);
  return distance2;
}

byte getBackDist()
{
  float volts1 = analogRead(LASER_SENSOR_R)*5.0/1024.0;
  distance1 = 65.0 * pow(volts1, -1.10);
  return distance1;  
  
}



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

void steer(byte dir){
 switch(dir){
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


void loop()                       // run over and over again
{
  laser();
  //Serial.println(getFrontDist());  
/*  obstacle=false;
  for(int i=FRONT_VAL-CRIT_RANGE/2;i<FRONT_VAL+CRIT_RANGE/2;i++)
  {
    if(frontDist[i]<50)
    {
      obstacle=true;
      if(!isSteering)
      {
        isSteering=true;
        steer(LEFT);
      }
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
*/
  delay(LOOP_DELAY); 
}
