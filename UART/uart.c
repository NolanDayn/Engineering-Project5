#include "stm32l476xx.h"
#include <stdarg.h>
#include <stdio.h>
#include "uart.h"

/*
	Implementation - for transmitting messages over UART1
	By: Nolan Thomas and Stephane Durette
	Jan 2020
	USART_Init Code borrowed from Embedded Systems with ARM Cortex-M
	Microcontrollers in Assembly Language and C By Dr.Yifeng Zhu
*/

//char buffer[512];

//Takes in a character and outputs it onto USART1
void UARTputC(char cx){

	while(!(USART1->ISR & USART_ISR_TXE));
	USART1->TDR = (uint16_t)cx;
	
}

//Behaves like a printf function but instead of outputting the string to 
//the console. It is output through USART 1 serial tx
void UARTprintf(char *fmt, ...){
	va_list args;	//Allows for a range of function inputs
	
	char buffer[100];

	va_start(args, fmt); //start going through the 
	vsnprintf(buffer, 100, fmt, args); //put provided variables into string as specified by fmt string
	va_end(args);
	
	UARTputs(buffer);
}

//Outputs a string ono USART1
void UARTputs(char *str){

	while(*str){
		UARTputC(*str++);
	}

}


/*Takes in a USART and enables it for use transmitting and receiving
* Sets data bits = 8
* Stop bit = 1
* parity cointrol = no parity control
* oversampling = 16
* buad rate = 9600
*/
void USART_Init(USART_TypeDef * USARTx){
	
	//Disable USART
	USARTx->CR1 &= ~USART_CR1_UE;
	
	//set the data length to 8 bits
	//00 = 8 data bits, 01 = 9 data bits, 10 = 7 data bits
	USARTx->CR1 &= ~USART_CR1_M;
	
	//Select 1 stop bit
	//00 = 1 Stop Bits	01 = 0.5 
	//10 = 2 stop bits	11 = 15 stop bits
	USARTx->CR2 &= ~USART_CR2_STOP;
	
	//set parity control as no parity
	// 0 = no parity
	// 1 = parity set
	USARTx->CR1 &= ~USART_CR1_PCE;
	
	//oversampling by 16
	//0 = oversamplying by 16, 1 = oversampling by 8
	USARTx->CR1 &= ~USART_CR1_OVER8;
	
	//set baud rate to 9600 using APB frequency (80MHz)
	USARTx->BRR = 0x208D;
	
	//enable transmission and reception
	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	
	//enable USART
	USARTx->CR1 |= USART_CR1_UE;
	
	//verify that the USART i ready for transmission
	//TEACK: Transmist enable acknowledge flag. Set or reset
	while((USARTx->ISR & USART_ISR_TEACK) == 0);
	
	//verify that USART is ready for reception
	//REACK: Recevie enable acknoweldge flag. Hardware sets or resets
	while((USARTx->ISR & USART_ISR_REACK) == 0);
	
}
