//-------------------------------------------------------------------
// Hack demo to toggle red and green LEDS on Discovery board
// Jan 2019 - WPS
//
// Uses Zhu's SysClock code to init system clocks
//-------------------------------------------------------------------

#include "stm32l476xx.h"
#include "SysClock.h"
#include "StepperMotor.h"
#include "joystick.h"

// delay count between changing LED states
#define DELAY_COUNT 10000000UL


int main(void){

	System_Clock_Init(); 	// Switch System Clock = 80 MHz
	InitStepper();
	JoystickInit();
	
	uint16_t currentPos;
	volatile int16_t value = 0;
	int stepSize;
	
	
	// loop forever toggling LEDs 
	for(;;) {
		
		value = GetJoystickButtonPressed();
		
		switch(value){
			case(1):
				stepSize = -1;
				break;
			case(2):
				stepSize = 1;
				break;
			case(3):
				stepSize = 2;
				break;
			case(5):
				stepSize = -2;
				break;
			default:
				stepSize = 0;
				break;
		}
		
		currentPos = TakeAStep(stepSize);
		
		for(int i = 0; i < 1000000; i++);
	}
	
}




