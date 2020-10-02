#include <stdio.h>

uint32_t inputPins[] = {0, 1, 2, 3, 5};
uint32_t sz_inputPins = 5;

void JoystickInit(){
	RCC-> AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //enable clock of port A
	
	//All pins are input pins.
	for(uint16_t i = 0; i < sz_inputPins; i++){	
		GPIOA->MODER &= (~3UL << (2 * inputPins[i]));
	}
	
	GPIOA-> PUPDR &= ~3UL; //the center button (0) is no pull-up, no pull-down [00]
	
	//All direction buttons are pull downs [10]
	for(uint16_t i = 1; i < sz_inputPins; i++){	
		GPIOA->PUPDR |= (2UL << (2 * inputPins[i]));
		GPIOA->PUPDR &= ~(1UL << (2 * inputPins[i])); 
	}
}

//get the number of the button pressed pin or -1
int16_t GetJoystickButtonPressed(){
	for(uint16_t i = 0; i < sz_inputPins; i++){
		if(GPIOA->IDR & (1UL << inputPins[i])){
			return inputPins[i];
		}
	}
	return -1;
}

