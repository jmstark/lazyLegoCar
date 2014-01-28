#include <Servo.h>
#include <SharpIR.h>

#include "serial_cmd.h"

//pins and constants
#define LASER_STEERING_FRONT 12
#define LASER_STEERING_BACK 2
#define MOTORS_I 7    //Digital-Steering
#define MOTORS_II 8   //Digital-Steering
#define MOTOR_I 5    //Propulsion FrontDir
#define MOTOR_II 6  //Propulsion BackDir
#define SPEED_MOTOR_A 9//H-Bridge
//#define LASER_SENSOR_R A0  //Analog
//#define LASER_SENSOR_B A1 //Analog
//#define LASER_SENSOR_0 0
//#define LASER_SENSOR_1 1
//#define LASER_SENSOR_2 2
//#define LASER_SENSOR_3 3
//#define LASER_SENSOR_4 4
#define LASER_SENSOR_B 5





SharpIR backIR = SharpIR(GP2Y0A02YK,LASER_SENSOR_B);  //red
SharpIR frontIR0 = SharpIR(GP2Y0A02YK,0); 
SharpIR frontIR1 = SharpIR(GP2Y0A02YK,1); 
SharpIR frontIR2 = SharpIR(GP2Y0A02YK,2); 
SharpIR frontIR3 = SharpIR(GP2Y0A02YK,3); 
SharpIR frontIR4 = SharpIR(GP2Y0A02YK,4); 
Servo frontServo, backServo;
uint8_t serialData[5];



void setup()                    // run once, when the sketch starts
{
  //Basic setup
  Serial.begin(9600);// set up Serial library at 9600 bps
  frontServo.attach(LASER_STEERING_FRONT);
  backServo.attach(LASER_STEERING_BACK);
  pinMode(MOTOR_I, OUTPUT);
  pinMode(MOTOR_II, OUTPUT);
  pinMode(SPEED_MOTOR_A, OUTPUT);
  digitalWrite(SPEED_MOTOR_A, HIGH); 
}


void loop()
{
  serialData[0]=getNextByte();
  switch(serialData[0])
  {
    case CMD_DRIVE_STOP:
      drive(DIR_STOP);
      break;
    case CMD_DRIVE_FWD:
      drive(DIR_FWD);
      break;
    case CMD_DRIVE_RWD:
      drive(DIR_RWD);
      break;
    case CMD_STEER_LEFT:
      steer(LEFT);
      break;
    case CMD_STEER_STRAIGHT:
      steer(STRAIGHT);
      break;
    case CMD_STEER_RIGHT:
      steer(RIGHT);
      break;
    case CMD_MOVE_LASER_FRONT:
      frontServo.write(getNextByte());
      break;
    case CMD_MOVE_LASER_BACK:
      backServo.write(getNextByte());
      break;
    case CMD_GET_LASERDATA_FRONT:
      serialData[0]=frontIR0.getData();
      serialData[1]=frontIR1.getData();
      serialData[2]=frontIR2.getData();
      serialData[3]=frontIR3.getData();
      serialData[4]=frontIR4.getData();
      Serial.write(serialData,5);
      break;
    case CMD_GET_LASERDATA_BACK:
      Serial.write(backIR.getData());
      break;
    default:
      break;
  }
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

//blocks until serial data is available and then returns the next byte.
uint8_t getNextByte()
{
  while(Serial.available()==0)
    {
    }//wait
  return Serial.read();
}
