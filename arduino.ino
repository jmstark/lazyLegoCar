#include <Servo.h>


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
#define STEP_DEGREES 10
#define NUM_STEPS (180/STEP_DEGREES)


byte frontDist[NUM_STEPS+1];
byte backDist[NUM_STEPS+1];
float distance2;
float distance1;

byte increment=1;
byte stepCnt=0;
//SharpIR front = SharpIR(GP2Y0A2YK,0);
//SharpIR back = SharpIR(GP2Y0A2YK,1);

Servo s;
//DistanceGP2Y0A21YK Dist;
int inc=1;

void drive(int dir);

void setup()                    // run once, when the sketch starts
{
  Serial.begin(9600);           // set up Serial library at 9600 bps
  s.attach(LASER_STEERING);
  //Dist.begin(A1);
  Serial.println("Hello world!");
  s.write(90);
  pinMode(MOTOR_I, OUTPUT);
  pinMode(MOTOR_II, OUTPUT);
  pinMode(SPEED_MOTOR_A, OUTPUT);
  digitalWrite(SPEED_MOTOR_A, HIGH); ///
  drive(DIR_FWD);
}

void laserSteering(){
  s.write(stepCnt*STEP_DEGREES);
  stepCnt += increment;
  if(stepCnt > NUM_STEPS-1 || stepCnt<=0){
    increment=-increment;
  }
  for(byte i=0;i<NUM_STEPS;i++)
    {
    Serial.print(frontDist[i]);
    Serial.print(' ');
    }
  Serial.println();
}

void laserData(){
  float volts1 = analogRead(LASER_SENSOR_R)*5.0/1024.0;
  float volts2 = analogRead(LASER_SENSOR_B)*5.0/1024.0;
  distance1 = 65.0 * pow(volts1, -1.10);
  distance2 = 65.0 * pow(volts2, -1.10);
  frontDist[stepCnt]=distance2;
  backDist[stepCnt]=distance1;
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

void loop()                       // run over and over again
{
  laserData();
  laserSteering();

  
  //Serial.print("Blue Sensor\t"); 
  //Serial.println(distance2);
  //Serial.print("Red Sensor\t");
  //Serial.println(distance1);
  //if(distance2 < 30)
  //  drive(STOP);
  //if(distance2 > 29) 
  //  drive(1)
  delay(300); 
}
