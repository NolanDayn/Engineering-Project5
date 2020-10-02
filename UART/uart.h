#include "stm32l476xx.h"
#include <stdarg.h>
#include <stdio.h>

/*
	Interface - for transmitting messages over UART1
	By: Nolan Thomas and Stephane Durette
	Jan 2020
	USART_Init Code borrowed from Embedded Systems with ARM Cortex-M
	Microcontrollers in Assembly Language and C By Dr.Yifeng Zhu
*/

#ifndef __UART__H
#define __UART__H

//Takes in a character and outputs it onto USART1
void UARTputC(char cx); 

//Outputs a string ono USART1
void UARTputs(char *str);

/*Behaves like a printf function but instead of outputting the string to 
* the console. It is output through USART 1 serial tx
*/
void UARTprintf(char *fmt, ...);

/*Takes in a USART and enables it for use transmitting and receiving
* Sets data bits = 8
* Stop bit = 1
* parity cointrol = no parity control
* oversampling = 16
* buad rate = 9600
*/
void USART_Init(USART_TypeDef * USARTx);
#endif

