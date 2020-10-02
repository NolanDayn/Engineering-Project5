#ifndef _SERVO_H
#define _SERVO_H

#include "stm32l476xx.h"
#include "utils.h"

#define SERVO_PIN 8

void InitServo( void );
void SetServoDutyCycle(uint32_t ds);

#endif



