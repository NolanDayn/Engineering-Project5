#include "UART.h"
#include "utils.h"

#define BufferSize 32
uint8_t USART2_Buffer_Rx[BufferSize];
volatile uint32_t pRx_Counter = 0;

// Derived from Zhu's Virtual Serial Port demo code
// USART2 connected to ST Link on Nucleo-64 STM32F303RE
//  WPS 30 Apr 2020

// to do
//	- clear comms errors while waiting to read 
//	- clean-up header
//	- tighten structure & eliminate scrap code


// UART Ports:
// ===================================================
// PA.2 = USART2_TX (AF7)  |  PA.3 = USART2_RX (AF7)

void UART2_Init(void) {
	// Enable the clock of USART  2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN ;  // Enable USART 2 clock		
	
	// Select the USART2 clock source
	// 00: PCLK selected as USART2 clock
	// 01: System clock (SYSCLK) selected as USART2 clock
	// 10: HSI16 clock selected as USART2 clock
	// 11: LSE clock selected as USART2 clock
	//RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;
	//RCC->CCIPR |=  RCC_CCIPR_USART2SEL_0;
	FORCE_BITS( RCC->CFGR3, RCC_CFGR3_USART2SW, RCC_CFGR3_USART2SW_SYSCLK );
	
	UART2_GPIO_Init();
	USART_Init(USART2);
	
	USART2->CR1 |= USART_CR1_RXNEIE; //Receive register not empty interrupt
	USART2->CR1 &= ~USART_CR1_TXEIE;	//Transmit register empty interrupt
	NVIC_SetPriority(USART2_IRQn, 0);			// Set Priority to 1
	NVIC_EnableIRQ(USART2_IRQn);					// Enable interrupt of USART1 peripheral
}

void UART2_GPIO_Init(void) {
	
	// Enable the peripheral clock of GPIO Port
	RCC->AHBENR |=   RCC_AHBENR_GPIOAEN;
	
	// ********************** USART 2 ***************************
	// PA.2 = USART2_TX (AF7)
	// PA.3 = USART2_RX (AF7)
	// Alternate function, High Speed, Push pull, Pull up
	// **********************************************************
	// Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOA->MODER   &= ~(0xF << (2*2));	// Clear bits
	GPIOA->MODER   |=   0xA << (2*2);      		
	GPIOA->AFR[0]  |=   0x77<< (4*2);       	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR &=   ~(0xF<<(2*2)); 					 	
	// GPIO Push-Pull: No pull-x (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR   &= ~(0xF<<(2*2));  				
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER  &=  ~(0x3<<2) ;       	
}


void USART_Init (USART_TypeDef * USARTx) {
	// Default setting: 
	//     No hardware flow control, 8 data bits, no parity, 1 start bit and 1 stop bit		
	USARTx->CR1 &= ~USART_CR1_UE;  // Disable USART
	
	// Configure word length to 8 bit
	USARTx->CR1 &= ~USART_CR1_M;   // M: 00 = 8 data bits, 01 = 9 data bits, 10 = 7 data bits
	
	// Configure oversampling mode: Oversampling by 16 
	USARTx->CR1 &= ~USART_CR1_OVER8;  // 0 = oversampling by 16, 1 = oversampling by 8
	
	// Configure stop bits to 1 stop bit
	//   00: 1 Stop bit;      01: 0.5 Stop bit
	//   10: 2 Stop bits;     11: 1.5 Stop bit
	USARTx->CR2 &= ~USART_CR2_STOP;   
                                    
	// CSet Baudrate to 9600 using APB frequency (72,000,000 Hz)
	// If oversampling by 16, Tx/Rx baud = f_CK / USARTDIV,  
	// If oversampling by 8,  Tx/Rx baud = 2*f_CK / USARTDIV
  // When OVER8 = 0, BRR = USARTDIV
	// USARTDIV = 72MHz/9600 = 7500
	// BRR limited to 16 bits
	USARTx->BRR = SystemCoreClock/BAUD_RATE;
	//USARTx->BRR = 7091;
	
	USARTx->CR1  |= (USART_CR1_RE | USART_CR1_TE);  	// Transmitter and Receiver enable
	
  if (USARTx == UART4){	
		USARTx->CR1 |= USART_CR1_RXNEIE;  			// Received Data Ready to be Read Interrupt  
		USARTx->CR1 &= ~USART_CR1_TCIE;    			// Transmission Complete Interrupt 
		USARTx->CR1 &= ~USART_CR1_IDLEIE;  			// Idle Line Detected Interrupt 
		USARTx->CR1 &= ~USART_CR1_TXEIE;   			// Transmit Data Register Empty Interrupt 
		USARTx->CR1 &= ~USART_CR1_PEIE;    			// Parity Error Interrupt 
		USARTx->CR1 &= ~USART_CR2_LBDIE;				// LIN Break Detection Interrupt Enable
		USARTx->CR1 &= ~USART_CR3_EIE;					// Error Interrupt Enable (Frame error, noise error, overrun error) 
		//USARTx->CR3 &= ~USART_CR3_CTSIE;				// CTS Interrupt
	}

//	if (USARTx == USART2){
//		USARTx->ICR |= USART_ICR_TCCF;
//		USART1->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
//	}
	
	USARTx->CR1  |= USART_CR1_UE; // USART enable                 
	
	while ( (USARTx->ISR & USART_ISR_TEACK) == 0); // Verify that the USART is ready for reception
	while ( (USARTx->ISR & USART_ISR_REACK) == 0); // Verify that the USART is ready for transmission
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	volatile uint8_t RxByte;
	// SR_RXNE (Read data register not empty) bit is set by hardware
	// need to check for comm error and clear here inside wait loop
	//while (!(USARTx->ISR & USART_ISR_RXNE)){}  // Wait until RXNE (RX not empty) bit is set

	// Reading USART_DR automatically clears the RXNE flag 		
	// !!!!! KLUDGE !!!!! virtual com port was sending ASCII with msb always set
	//                    mask-off with 0x7F to get valid characters
		RxByte = USART2_Buffer_Rx[0];// read first item on buffer
	
		//shift whole buffer down to remove first item
		for(int i = 1; i < BufferSize; i++){ 
			USART2_Buffer_Rx[i-1] = USART2_Buffer_Rx[i];
		}
		//move the buffer counter back a spot
		if(pRx_Counter > 0){
			pRx_Counter--;
		}
		return (RxByte);

}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	//USARTx->ISR &= ~USART_ISR_TC;
	USARTx->ISR = USART_ISR_TC;// WPS - RmW issue?
}   
 

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}

void receive(USART_TypeDef *USARTx, uint8_t *buffer, uint32_t *pCounter){
	if(USARTx->ISR & USART_ISR_RXNE) {						// Received data                         
			buffer[*pCounter] = USARTx->RDR;         // Reading USART_DR automatically clears the RXNE flag 
			(*pCounter)++;  
			if((*pCounter) >= BufferSize )  {
				(*pCounter) = 0;
			}   
	}
}

void USART2_IRQHandler(){
	receive(USART2, USART2_Buffer_Rx, &pRx_Counter);
}


/*void USART_IRQHandler(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t * pRx_counter){
	if(USARTx->ISR & USART_ISR_RXNE) {						// Received data                         
		buffer[*pRx_counter] = USARTx->RDR;         // Reading USART_DR automatically clears the RXNE flag 
		(*pRx_counter)++;  
		if((*pRx_counter) >= BufferSize )  {
			(*pRx_counter) = 0;
		}   
	} else if(USARTx->ISR & USART_ISR_TXE) {
 		//USARTx->ISR &= ~USART_ISR_TXE;            // clear interrupt 
		//Tx1_Counter++;
	} else if(USARTx->ISR & USART_ISR_ORE) {			// Overrun Error
		while(1);
	} else if(USARTx->ISR & USART_ISR_PE) {				// Parity Error
		while(1);
	} else if(USARTx->ISR & USART_ISR_PE) {				// USART_ISR_FE	
		while(1);
	} else if (USARTx->ISR & USART_ISR_NE){ 			// Noise Error Flag
		while(1);     
	}	
}*/

