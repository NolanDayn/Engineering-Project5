#ifndef __STEPPER_MOTOR_H__
#define __STEPPER_MOTOR_H__

#include "stm32l476xx.h"

static uint8_t stepState = 0;
uint16_t TakeAStep(int16_t stepAmount);
void InitStepper(void);
																			
#endif



																			