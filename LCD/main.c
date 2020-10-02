#include "stm32l476xx.h"
#include "SysClock.h"
#include "lcd.h"
#include <stdarg.h>

#define FOREVER for(;;)
#define DELAY		for(int i = 0; i < 1000000; i++)

int main(void){

	System_Clock_Init(); // Switch System Clock = 80 MHz
	LCDinit();
	uint32_t count = 0;
	
	
	
	FOREVER{
		LCDprintf("SD NT OMG\n%04X", count++);
		DELAY;
		LCDclear();
	}
}


