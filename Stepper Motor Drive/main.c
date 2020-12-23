//-------------------------------------------------------------------
// Shell code for Nucleo-64 STM32F303RE @ 72 MHz
//
// Changes Green (user) LED state in response to character commands
//	received over ST Link Virtual COM port
//
// Alternate version flashes green LED 
//
// WPS 30 Apr 2020 - from Zhu Virtual Com Port demo code
//
//-------------------------------------------------------------------

#include "stm32f303xe.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "StepperMotor.h"

#include <stdio.h>

// delay count between changing LED states
#define DELAY_COUNT 1000000UL


int main(void){

	System_Clock_Init(); 	// Switch System Clock = 80 MHz
	SystemCoreClockUpdate();
	LED_Init();						// Init GPIO bits to drive LEDs
  InitStepper();
	InitStepperLimits();
	
	for(;;) {	
		TakeAStep(1);
		int left = LeftLimitHit();
		int right = RightLimitHit();
		for( uint32_t i = 0; i < DELAY_COUNT; i++);
	} 
}

