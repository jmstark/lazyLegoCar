/*
  Code for autonomous driving of LEGO Car. This Piece of code implements 
  a basic obstacle avoidance behaviour while trying to drive forward.

  TUM - SS 2013 - Hardware/Software Co-Design Lab Course 
  (Supervisor: Kai Huang)
  
  AUTHORS: Lars Kipferl, Hannes Schulz, Sebastian Fischer, Yanis Chiotellis

*/

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <math.h>
#include <float.h>

/// GLOBAL VARIABLES ///
		
int laser_angles[19] = {0};	// variables to store and manage laser data
int laser_frontDist[19] = {320};
int laser_backDist[19] = {320};
int arraySize = 19;
int angle_index = 0;
int front_index = 0;
int back_index = 0;

double attForce[2];	// variable for attracting force
double repForce[2];	// variable for repulsive force
double resForce[2];  	// variable for resulting force

bool drive_mode = false;	// switches between scanning and driving

int drive = 0;	// Integers for sending drive commands to arduino
int steer = 0;

double maxInf = 80.0; // maximum distance (values over 80cm are ommited)
int scalFact = 27000; // scaling factor for calculating forces

double steerToRight = -0.01;	// parameter which decides when to steer right
double steerToLeft = 0.01;	// parameter which decides when to steer to left

// Calculates the attractive force (currently set to fixed value)
void calcAttForce() {
  attForce[0] = 1.0;
  attForce[1] = 0.0;
}

// Calculates the repulsive force by using laser_data
void calcRepForce() {
  bool go_on = false; // bool to decide wether other calculations are necessary 
  repForce[0] = 0;
  repForce[1] = 0;
  // Drive forward if no obstacles are found in a 40 to -40 degree radius
  for(int i=0;i<arraySize;i++) {
    if(laser_angles[i] > -40 && laser_angles[i] < 40) {
      if(laser_frontDist[i] < maxInf) { // found obstacle? ...
        go_on = true; // ... then we need to calculate the repulsive force of it
        break;
      } 
    }
  }
  if(go_on) { // obstacle found in 40 to -40 degree radius? -> calculate repForce
    // Search for nearest obstacle ...
    double minDist = DBL_MAX;
    int angle_ind;
    for(int i=4;i<arraySize-4;i++) { /// ... in 40 to -40 radius
      if(laser_frontDist[i] < minDist) {
        minDist = laser_frontDist[i];
        angle_ind = i;
      }
    }
    // now calculate the repulsive force of the obstacle
    double rad = laser_angles[angle_ind]*(M_PI/180); // need rad for cos() and sin() function
    double x = minDist*cos(rad); // cartesian coordinates of the obstacle
    double y = minDist*sin(rad);
    if(minDist < maxInf && minDist != 0) {
      repForce[0] = scalFact*((1/minDist)-(1/maxInf))*(1/powf(minDist,2))*(-x/minDist);
      repForce[1] = scalFact*((1/minDist)-(1/maxInf))*(1/powf(minDist,2))*(-y/minDist);
    }
  }
}

// Calculates the resulting force by adding all attracting and repulsive forces
void calcResForce() {
  resForce[0] = attForce[0] + repForce[0];
  resForce[1] = attForce[1] + repForce[1];
}

// Decide which motor controls to send
void discretize() {
  if(resForce[1] <= steerToRight) { // if certain treshold is surpassed, steer right ...
    steer = 2;
  } else if(resForce[1] > steerToLeft) { // ... same for the left side ...
    steer = 1;
  } else { // ... or just drive straight when nothing is happening
    steer = 0;
  }
  if(resForce[0] <= 0.0) { // if obstacles are too near, drive backwards ...
    drive = 2;
    switch(steer) { // ... and steer in the opposite direction
      case(2): steer = 1;
      break; 
      case(1): steer = 2;
      break; 
      case(0): steer = 2;
    }
  } else {
    drive = 1; // if the path ahead is clear, drive forward
  }
  ROS_INFO("steer: %d  drive: %d", steer, drive); // outputs on console which driving commands are sent
}

// Callback for servo motor (angle) subscriber
void laserAngleCallback(const std_msgs::Int32::ConstPtr& laserAngle_msg)
{
  // Fill laser data with one full scan, then begin overwriting old data
  if(angle_index >= 19) {
    angle_index = 0;
    drive_mode = !drive_mode; // alternate between driving and scanning for the duration of one scan each 
  }
  laser_angles[angle_index] = laserAngle_msg->data;
  laser_angles[angle_index] -= 90; // angles should be represented in right-hand-coordinate system
  angle_index++;
}

// Callback for front IR subscriber
void laserFrontDistCallback(const std_msgs::Int32::ConstPtr& laserFrontDist_msg)
{
  if(front_index >= 19) {
    front_index = 0;
  }
  laser_frontDist[front_index] = laserFrontDist_msg->data;
  front_index++;
}

// Callback for back IR subscriber
void laserBackDistCallback(const std_msgs::Int32::ConstPtr& laserBackDist_msg)
{
  if(back_index >= 19) {
    back_index = 0;
  }
  laser_backDist[back_index] = laserBackDist_msg->data;
  back_index++;
}

int main(int argc, char **argv)
{
  // initialize node
  ros::init(argc, argv, "potentialfield");
  
  // nodehandler keeps track of wether the node is still running or not
  ros::NodeHandle n;

  // set up subscriber to get data from arduino
  ros::Subscriber angle_sub = n.subscribe("radar_angle", 1, laserAngleCallback);
  ros::Subscriber frontDist_sub = n.subscribe("radar_front", 1, laserFrontDistCallback);
  ros::Subscriber backDist_sub = n.subscribe("radar_back", 1, laserBackDistCallback);

  // set up publisher to send data to arduino 
  ros::Publisher drive_pub = n.advertise<std_msgs::Int32>("directions_drive", 1);
  ros::Publisher steer_pub = n.advertise<std_msgs::Int32>("directions_steer", 1);
  // loop_rate determines how often processes are called in hertz
  ros::Rate loop_rate(10);

  int count = 0; // variable to count loop iterations (used internally by ros)
  while(ros::ok())
  {
    // messages to be published by publisher
    std_msgs::Int32 drive_msg;
    std_msgs::Int32 steer_msg;
   
    // scan if not driving, drive when not scanning
    if(!drive_mode) {
      calcAttForce();
      calcRepForce();
      calcResForce();
      discretize();
      drive_msg.data = 0; // sets the message data
    } else {
      drive_msg.data = drive; 
    }
    steer_msg.data = steer;
    
    // publish messages to topic
    drive_pub.publish(drive_msg);
    steer_pub.publish(steer_msg);    
  
    // Pump Callbacks
    ros::spinOnce();

    // Wait (Depends on loop_rate)
    loop_rate.sleep();
    ++count;
  }		
  return 0;
}
