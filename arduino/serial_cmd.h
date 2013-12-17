#ifndef SERIAL_CMD_H
#define SERIAL_CMD_H


const uint8_t DIR_STOP=0;
const uint8_t DIR_FWD=1;
const uint8_t DIR_RWD=2;
const uint8_t STRAIGHT=3;
const uint8_t LEFT=4;
const uint8_t RIGHT=5;


const uint8_t CMD_DRIVE_STOP=DIR_STOP;
const uint8_t CMD_DRIVE_FWD=DIR_FWD;
const uint8_t CMD_DRIVE_RWD=DIR_RWD;
const uint8_t CMD_STEER_LEFT=LEFT;
const uint8_t CMD_STEER_STRAIGHT=STRAIGHT;
const uint8_t CMD_STEER_RIGHT=RIGHT;
const uint8_t CMD_MOVE_LASER_FRONT=6;
const uint8_t CMD_MOVE_LASER_BACK=7;
const uint8_t CMD_GET_LASERDATA_FRONT=8;
const uint8_t CMD_GET_LASERDATA_BACK=9;


#endif
