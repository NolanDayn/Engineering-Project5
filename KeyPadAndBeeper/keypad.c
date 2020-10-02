#include "keypad.h"
#include "LED.h"
#include "utils.h"

uint8_t len = 4;
uint32_t outputs[] = {12, 13, 14, 15};
uint32_t inputs[] = {1, 2, 3, 5};

//Initializes pins used as keyboard columns and rows
void InitKeypad(){
	
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOEEN;
	
	for(uint8_t i = 0; i < len; i++){
		//Initialize the rows
		FORCE_BITS(GPIOE -> MODER, 3UL << (2 * outputs[i]), 1UL << (2 * outputs[i])); //digital output = 01
		FORCE_BITS(GPIOE -> OTYPER, 1UL << (outputs[i]), 1UL << (outputs[i])); //open drain = 1, push pull = 0
		FORCE_BITS(GPIOE -> OSPEEDR, 3UL << (2 * outputs[i]), 0 << (2 * outputs[i])); //low speed = 00
		FORCE_BITS(GPIOE -> PUPDR, 3UL << (2 * outputs[i]), 1UL << (2 * outputs[i])); //pull up = 01
		
		//Initialize the columns
		FORCE_BITS(GPIOA -> MODER, 3UL << (2 * inputs[i]), 0 << (2 * inputs[i])); //input mode = 00
		FORCE_BITS(GPIOA -> PUPDR, 3UL << (2 * inputs[i]), 1UL << (2 * inputs[i])); //pull up= 01
	}

}

//When ODR is 0x7, IDR is always 0x7
/* Read key input from keypad by cycling through rows and connecting them to
*  ground through open drain. Then cycling through the columns and checking which is
*  low. Returns an enum representing pressed key by column and row.
*/
enum Keys GetKey( void ){
	uint8_t outputValues[] = {0x7, 0xB, 0xD, 0xE};
	int keyIndex = -1;
	
	for (int i = 0; i < len; i++){
		
		FORCE_BITS(GPIOE->ODR, 0xF << outputs[0], outputValues[i] << outputs[0]);
		uint8_t idr = DebouncedKeyInputs(); //get debounced input
		
		for (int j = 0; j < len; j++){
			if (idr == outputValues[j]){
				keyIndex = i*len + j;
				return (enum Keys)(keyIndex); //convert loop variables into key enum
			}
		}
	}
	return Key_None;
}

/* GetKeyPadInputs 
*  Checks the value in the columns of the keypad and returns 
*  which columns are currently on
*/
uint8_t GetKeyPadInputs(){
	  volatile uint8_t Idr1 = (GPIOA -> IDR & 1UL << 1) >> 1;
		volatile uint8_t Idr2 = (GPIOA -> IDR & 1UL << 2) >> 2;
		volatile uint8_t Idr3 = (GPIOA -> IDR & 1UL << 3) >> 3;
		volatile uint8_t Idr5 = (GPIOA -> IDR & 1UL << 5) >> 5;
		return Idr1 | (Idr2 << 1) | (Idr3 << 2) | (Idr5 << 3);
}

/*Reads key pad inputs a given number of times with a small delay in
* between. If the key read is not the same as the first read one of the times
* within the count return no key pressed otherwise return value of keypress.
* Some what helps to debounce input on the keypad
*/
uint8_t DebouncedKeyInputs(){
	
	uint8_t idr;
	idr = GetKeyPadInputs();
	
	for(int i = 0; i < DEBOUNCE_COUNTER; i++){
		
		uint8_t newReading = GetKeyPadInputs();
		if(idr != newReading || idr == 0xF){
			idr = 0xF;
			return idr;
		}
		
		DEBOUNCE_DELAY;
	}
	
	return idr;
}

