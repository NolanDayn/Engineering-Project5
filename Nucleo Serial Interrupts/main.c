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

#include <stdio.h>

// delay count between changing LED states
#define DELAY_COUNT 1000000UL

#if 0

int main(void){

	System_Clock_Init(); 	// Switch System Clock = 80 MHz
	LED_Init();						// Init GPIO bits to drive LEDs

	// loop forever toggling LEDs 
	for(;;) {
		
		Green_LED_Toggle();
			
		for( uint32_t i = 0; i < DELAY_COUNT; i++);
		
	} // for
	
} //main()

#endif


#if 1

int main(void){

	System_Clock_Init(); 			// Switch System Clock = 72 MHz
	SystemCoreClockUpdate();	// update global var SystemCoreClock to new clock value (72 MHz)
	
	LED_Init();
	UART2_Init();
	
		//printf("\r\n\r\nSTM32F303RE Nucleo-64 LED & Virtual Com port demo\r\n");
		//printf("LED is On\r\n\r\n");
		Green_LED_On();	
	
//printf("0");	
//			for( uint32_t i = 0; i < DELAY_COUNT; i++);
						
	while (1){
		
		char cmd[5];
		char rxByte;
		USART_Read_Cmd (USART2, cmd, 5);

		
		printf("%c%c%c%c%c\n", cmd[0], cmd[1], cmd[2], cmd[3], cmd[4]);

		for(int i = 0; i < 1000000; i++){}
		
	} // while
	
}	// main()
#endif

