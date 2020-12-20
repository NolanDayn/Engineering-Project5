#include "Msg_Specs.h"
#include "GameController.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void CenterCmd(char* cmd){
    snprintf(cmd,CMD_LENGTH,"%c%c%c%c",'C','0','0','0');
}

void ServoCmd(char* cmd, int value){

    char direction = '+';
    if(value < 0){
        direction = '-';
        value = value *-1;
    }

    snprintf(cmd,CMD_LENGTH,"%c%c%c%d",'E',direction,'0',value);
}

void StepperCmd(char* cmd, int value){
    
    char direction = '+';
    if(value < 0){
        direction = '-';
        value = value *-1;
    }

    snprintf(cmd,CMD_LENGTH,"%c%c%c%d",'T',direction,'0',value);
}


void MotorCmd(char cmd[CMD_BUF_SIZE][CMD_LENGTH], int* cmdNum, MotorData* motorData){

    const int MIN_INPUT = 10000;
    int motorScale = 1000;
    int leftM = 0;
    int rightM = 0;
    char direction = '+';
    
    motorData->y = motorData->y * -1; //Invert based on controller input being backwards to eyes

    if(motorData->y > MIN_INPUT || motorData->y < -1*MIN_INPUT){
        leftM = motorData->y;
        rightM = motorData->y;

        //right turn
        if(motorData->x > MIN_INPUT){
            rightM *= 0.9;
            leftM *= 1.1;
        }

        //left turn
        if(motorData->x < -1*MIN_INPUT){
            rightM *= 1.1;
            leftM *= 0.9;
        }

    }
    else{
          //right turn
        if(motorData->x > MIN_INPUT){
            rightM = -motorData->x/2;
            leftM = motorData->x/2;
        }

        //left turn
        if(motorData->x < -1*MIN_INPUT){
            rightM = motorData->x/2;
            leftM = -motorData->x/2;
        }
    }

    if(leftM < 0){
        direction = '-';
    }

    leftM = abs(leftM/motorScale);
    snprintf(cmd[*cmdNum], CMD_LENGTH, "L%c%02d", direction, leftM);
    *cmdNum = *cmdNum + 1;

    if(rightM < 0){
        direction = '-';
    }
    rightM = abs(rightM/motorScale);
    snprintf(cmd[*cmdNum], CMD_LENGTH, "R%c%02d", direction, rightM);
}