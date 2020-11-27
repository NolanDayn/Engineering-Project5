#ifndef __STM32F303RE_NUCLEO64_UART_H
#define __STM32F303RE_NUCLEO64_UART_H

#include "stm32f303xe.h"

void 		UART2_Init(void);
void 		UART2_GPIO_Init(void);

void 		USART_Init(USART_TypeDef * USARTx);
void 		USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes);
uint8_t USART_Read(USART_TypeDef * USARTx);
void 		USART_Delay(uint32_t us);

#define BAUD_RATE 9600

#endif
