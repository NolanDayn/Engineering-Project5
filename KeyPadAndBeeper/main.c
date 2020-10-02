#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "lcd.h"
#include "Beeper.h"
#include "Notes.h"
#include "keypad.h"

#define DELAY_COUNT 	10000000UL
#define DELAY 				for( uint32_t i = 0; i < DELAY_COUNT; i++)
#define FOREVER 			for(;;)

int toneFrequencies[16] = {C3, C5, B4, A4, G4, F4, E4, D4, C4, B3, A3, G3, F3, E3, D3, D5};

int main(void){
	enum Keys pressed;
	System_Clock_Init(); 	// Switch System Clock = 80 MHz
	LED_Init();						// Init GPIO bits to drive LEDs
	InitKeypad();					//Init beeper pin
	InitBeeper();					//Init keypad pin
	
	FOREVER {
		pressed = GetKey();
		if(pressed == Key_None){	//No Key
			StopBeep();
			Green_LED_On();
			Red_LED_Off();
		} else {									//A key was pressed
			Green_LED_Off();
			Red_LED_On();
			Beep(toneFrequencies[pressed]);
			DELAY;
		}
	}
} 

