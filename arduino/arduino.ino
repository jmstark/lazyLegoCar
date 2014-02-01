#include "pitches.h"
#include <Servo.h>
#include <SharpIR.h>

#include "serial_cmd.h"

//pins and constants
#define LASER_STEERING_FRONT 12
#define LASER_STEERING_BACK 4
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
#define BEEP 13
#define YELLOW_LED 3
#define RED_LED 2

int melodyPower[]={NOTE_C5,0,NOTE_C5};
int durationsPower[]={8,8,8};

int melodyConnect[]={NOTE_C4,0,NOTE_C5};
int durationsConnect[]={8,8,8};

int melodyDisconnect[]={NOTE_C5,0,NOTE_C4};
int durationsDisconnect[]={8,8,8};

int melodyPathStart[]={NOTE_C5,NOTE_E5, NOTE_G5};
int durationsPathStart[]={4,4,4};

int melodyPathEnd[]={NOTE_C6,0,NOTE_C6};
int durationsPathEnd[]={8,8,2}; 



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
  pinMode(YELLOW_LED,OUTPUT);
  pinMode(RED_LED,OUTPUT);
  pinMode(BEEP,OUTPUT);
  digitalWrite(SPEED_MOTOR_A, HIGH); 
  backServo.write(90);
  frontServo.write(90);
  //playMelody(melodyPower,durationsPower,sizeof(melodyPower));
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
    case CMD_RED_LED_ON:
      digitalWrite(RED_LED,HIGH);    
      //playMelody(melodyPathStart,durationsPathStart,sizeof(melodyPathStart));  
      break;
    case CMD_RED_LED_OFF:
      digitalWrite(RED_LED,LOW);      
      //playMelody(melodyPathEnd,durationsPathEnd,sizeof(melodyPathEnd));  
      break;
    case CMD_YELLOW_LED_ON:
      digitalWrite(YELLOW_LED,HIGH);      
      //playMelody(melodyConnect,durationsConnect,sizeof(melodyConnect));  
      break;
    case CMD_YELLOW_LED_OFF:
      digitalWrite(YELLOW_LED,LOW);      
      //playMelody(melodyConnect,durationsDisconnect,sizeof(melodyDisconnect));  
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


void playMelody(int* melody, int* durations, int size) {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < size; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/durations[thisNote];
    tone(BEEP, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BEEP);
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
