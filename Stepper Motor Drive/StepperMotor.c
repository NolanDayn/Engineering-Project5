#include "stm32f303xe.h"
#include "StepperMotor.h"

const uint8_t stepTable[8] = { 0x08, 0x0A, 0x02, 0x06, 0x04, 0x05, 0x01, 0x09};

uint16_t TakeAStep(int16_t stepAmount){
	
	uint8_t stepPatternOut;
	uint8_t nextStep = stepState + stepAmount;
	
	nextStep &= 0x07; //Clear all but last 3 bits to  prevent overflow, underflow and allow cycling
	stepPatternOut = stepTable[nextStep];
	
	//Write step state to GPIOs
	//GPIOC->ODR &= 0xFFF0; //Erase first 4 lowest bits in the output
	//GPIOC->ODR |= stepPatternOut;
	
	//Write step state to GPIOs
	GPIOC->ODR = (GPIOC->ODR & 0xFFF0) | stepPatternOut;
	
	stepState = nextStep;
	
	return stepState;
}

void InitStepper(){
	
	//Enable GPIO Clock for port E
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	
	//Enable PE12 to PE15 for GPIO output use in stepper motor driver L293D //PC0 to PC3 CN7-35 to 38
	for(int i = 0; i < 4; i++){
		// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
		GPIOC->MODER &= ~(3UL<<(2*i));  
		GPIOC->MODER |= (1UL<<(2*i));      //  Output(01)
		
		// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
		GPIOC->OSPEEDR &= (~(3UL<<(2*i)));
		GPIOC->OSPEEDR |=   (3UL<<(2*i));  // High speed
		
		// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
		//GPIOE->OTYPER &= ~(1UL<<i);       // Push-pull
		
		// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
		GPIOC->PUPDR   &= ~(3UL<<(2*i));  // No pull-up, no pull-down
	}
}


