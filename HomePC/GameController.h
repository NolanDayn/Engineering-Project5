#ifndef __GAME_CONTROLLER__H
#define __GAME_CONTROLLER__H

#include "Msg_Specs.h"

#define JS_EVENT_BUTTON 0x01 //Button pressed/released
#define JS_EVENT_AXIS 0x02   //Joystick moved
#define JS_EVENT_INIT 0x80   //Initial state of device

#define CENTER_BUTTON 0
#define STEPPER_AXIS 6
#define SERVO_AXIS 7
#define MOTOR_FORWARD_AXIS 1
#define MOTOR_TURN_AXIS 0

typedef struct js_event{
    unsigned int time;  //Event timestamp in milliseconds
    short value;        //value
    unsigned char type; //event type
    unsigned char number; //axis/button number
}js_event;

typedef struct MotorData{
    int x;
    int y;
}MotorData;

void ServoCmd(char* cmd, int value);
void CenterCmd(char* cmd);
void StepperCmd(char* cmd, int value);
void MotorCmd(char cmd[CMD_BUF_SIZE][CMD_LENGTH],  int* cmdNum, MotorData* motorData);

#endif