#include "LED.h"


//******************************************************************************************
// Nucleo-64 STM32F303RE User LED: 
//  LD5 Green = PA5
//******************************************************************************************
void LED_Init(void){
	/* Enable GPIOs clock */ 	
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	// LED Green = PA5
	///////////////////////////////////////////////////////////////////////////////////////////////
	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3U<<(2*5));  
	GPIOA->MODER |= 1U<<(2*5);      //  Output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR &= ~(3U<<(2*5));
	GPIOA->OSPEEDR |=   3U<<(2*5);  // High speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA->OTYPER &= ~(1U<<5);       // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR   &= ~(3U<<(2*5));  // No pull-up, no pull-down
}


//******************************************************************************************
// Turn Green LED On
//******************************************************************************************
void Green_LED_On(void){
	GPIOA->ODR |= GPIO_ODR_5;
}

//******************************************************************************************
// Turn Green LED Off
//******************************************************************************************
void Green_LED_Off(void){
	GPIOA->ODR &= ~GPIO_ODR_5;
}

//******************************************************************************************
// Toggle Green LED
//******************************************************************************************
void Green_LED_Toggle(void){
	GPIOA->ODR ^= GPIO_ODR_5;
}
