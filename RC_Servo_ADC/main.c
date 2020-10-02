//-------------------------------------------------------------------
// Hack demo to toggle red and green LEDS on Discovery board
// Jan 2019 - WPS
//
// Uses Zhu's SysClock code to init system clocks
//-------------------------------------------------------------------

#include "stm32l476xx.h"
#include "SysClock.h"
#include "ReadADC.h"
#include "lcd.h"
#include "servo.h"
#include <stdarg.h>

int main(void){

	System_Clock_Init(); // Switch System Clock = 80 MHz
	
	//Start initializations
	LCDinit();					//LCD
	InitServo();				//Servo
	TurnOnADC_CLK();		//ADC Clock
	PA1_AnalogConfig(); //Configure PA1 as analog pin
	Initialize_ADC1();	//Initialize ADC1
	
	for(;;){
		
		uint16_t adcReading = TriggerADCConversion();
		
		LCDprintf("ADC Reading \n%04X", adcReading);
		for(int i = 0; i < 1000000; i++);
		LCDclear();
	}
}

