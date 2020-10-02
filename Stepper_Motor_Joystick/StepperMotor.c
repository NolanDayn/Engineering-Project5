#include "stm32l476xx.h"
#include "StepperMotor.h"

const uint8_t stepTable[8] = { 0x08, 0x0A, 0x02, 0x06,
																			0x04, 0x05, 0x01, 0x09};

uint16_t TakeAStep(int16_t stepAmount){
	
	uint16_t stepPatternOut;
	uint8_t nextStep = stepState + stepAmount;
	
	nextStep &= 0x07; //Clear all but last 3 bits to  prevent overflow, underflow and allow cycling
	stepPatternOut = stepTable[nextStep] << 12;
	//stepPatternOut &= 0xF000; //Set every bit except last 4 in pattern to be 0
	
	//Write step state to GPIOs
	GPIOE->ODR &= 0x0FFF; //Erase first 4 highest bits in the output
	GPIOE->ODR |= stepPatternOut;
	
	stepState = nextStep;
	
	return stepState;
}

void InitStepper(){
	
	//Enable GPIO Clock for port E
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	
	//Enable PE12 to PE15 for GPIO output use in stepper motor driver L293D
	for(int i = 12; i < 16; i++){
		// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
		GPIOE->MODER &= ~(3UL<<(2*i));  
		GPIOE->MODER |= (1UL<<(2*i));      //  Output(01)
		
		// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
		GPIOE->OSPEEDR &= (~(3UL<<(2*i)));
		GPIOE->OSPEEDR |=   (3UL<<(2*i));  // High speed
		
			// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
		//GPIOE->OTYPER &= ~(1UL<<i);       // Push-pull
		
		// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
		GPIOE->PUPDR   &= ~(3UL<<(2*i));  // No pull-up, no pull-down
	}
}


