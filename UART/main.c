#include "stm32l476xx.h"
#include "SysClock.h"
#include "uart.h"
#include <stdarg.h>
#include <stdio.h>

/*	main.c - for uart lab 2 embedded systems software
* 	prints a simple one line message over UART1 on STM32L476 Discovery Board
*		By: Nolan Thomas and Stephane Durette
*		Jan 28, 2020
*/

int main(void){

	System_Clock_Init(); // Switch System Clock = 80 MHz
	
	
	//----Enable GPIO clock and configure the Tx and Rx Pins---
	//GPIO Initilization for USART1 
	RCC->AHB2ENR |=RCC_AHB2ENR_GPIOBEN;
	
	// 00 = Input, 01 = Output, 10 = Alternate Function, 11 = Analog
	GPIOB->MODER &= ~(0xF << (2*6)); //Clear mide bits for pin 6 and 7
	GPIOB->MODER |= 0xA << (2*6); // Select alternate function mode
	
	//Alternate function 7 = USART 1
	//Appendix I show all alternate functions
	GPIOB->AFR[0] |= 0x77 << (4*6); //set pins 6 and 7 to AF 7
	
	//GPIO Speed: 00 = Low speed, 01 = Medium Speed,
	//							10 = Fast Speed, 11 = High Speed
	GPIOB->OSPEEDR |= 0xF<<(2*6);
	
	//GPIO Push Pull: 00 = No pull- up/pull-down, 01 = Pull-up (01)
										//10 = pull-down, 11= Reserved
	GPIOB->PUPDR &= ~(0xF<<(2*6));
	GPIOB->PUPDR |= 0x5<<(2*6); //select puul-up
	
	
	//GPIO output type: 0 = push-pull, 1 = open drain
	GPIOB->OTYPER &= ~(0x3<<6);
	
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //Enable UART 1 clock
	//----------------------------------------------------------------
	
	USART_Init(USART1);
	
	uint32_t count = 0;
	for(;;){
		for(int i = 0; i < 100000; i++); //provide short delay between messages
		UARTprintf("Nolan Thomas\nStephane Durette\ncount: 0x%X\n", count++);
		
		while(!(USART1-> ISR & USART_ISR_TC)); //Waiting for transmission complete flag to be set before moving on
		USART1->ICR |= USART_ICR_TCCF;
	}
}

