#ifndef __TESTMENUS_H__
#define __TESTMENUS_H__

#include "stm32l476xx.h"
#include "SysClock.h"
#include "UART.h"

#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 1000
#define BUILD_DATE "April,26 2020"

#define SERVO_MAX_RANGE 90
#define SERVO_MIN_RANGE 0
#define	STEPPER_MAX_POS 180
#define	STEPPER_MIN_POS 0

void MainMenu(void);
int8_t checkInput(int input, int lowerBound, int upperBound);

void StepperMenu(void);
void ServoMenu(void);
void DCMotorMenu(void);
void EncoderMenu(void);
void LCDMenu(void);
void RS232Menu(void);
void IMUMenu(void);

#endif
