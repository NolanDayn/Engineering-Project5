#ifndef _SERVO_H
#define _SERVO_H

#include "stm32f303xe.h"
#include "utils.h"

#define SERVO_PIN 0

void InitServo( void );
void SetServoDutyCycle(uint32_t ds);

#endif



