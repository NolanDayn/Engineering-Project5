// Derived from Zhu's Virtual Serial port demo
// WPS 30 April 2020

// Maps low-level fgetc() & fput() functions to UART2 so higher-level stream functions work
// e.g.  printf(), scanf(), fgets(), fputs(), getchar(), etc

#include "UART.h"
#include <stdio.h>

// Implement a dummy __FILE struct, which is called with the FILE structure.
struct __FILE {
    int dummy;
};

// We have to define FILE if prinf is used
FILE __stdout;
FILE __stdin;
 
// remap fputc() to write USART2 by polling
int fputc(int ch, FILE *f) { 
  uint8_t c;
  c = ch & 0x00FF;
  USART_Write(USART2, (uint8_t *)&c, 1);
  return(ch);
}

// remap fgetc() to read USART2 by polling
int fgetc(FILE *f) {  
  uint8_t rxByte;
  rxByte = USART_Read(USART2);
  return rxByte;
}
