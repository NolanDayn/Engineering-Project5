#include "stm32l476xx.h"
#include "SysClock.h"
#include "UART.h"
#include "TestMenus.h"
#include <string.h>
#include <stdio.h>


char RxComByte = 0;
int main(void){

	System_Clock_Init(); // Switch System Clock = 80 MHz
	UART2_Init();
	
		
	while (1){
		MainMenu();
	}
}



