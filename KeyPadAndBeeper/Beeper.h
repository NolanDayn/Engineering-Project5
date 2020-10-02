#ifndef _BEEPER_H
#define _BEEPER_H
#include "stm32l476xx.h"

//Initiliaze the Piezo Beeper on Pin B6
void InitBeeper( void );

//Send signal to beeper to vibrate at
void Beep( uint32_t hertz );

//Turn off beeper
void StopBeep( void );

#endif

