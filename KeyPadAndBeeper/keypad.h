#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include "stm32l476xx.h"

#define ROWS 4
#define COLUMNS 4 
#define DEBOUNCE_COUNTER 3
#define DEBOUNCE_DELAY_AMOUNT 8000 
#define DEBOUNCE_DELAY for(int d = 0; d < DEBOUNCE_DELAY_AMOUNT; d++)

//represents every state of input from keypad
enum Keys {Key_None = -1, Key_1, Key_2, Key_3, Key_A, Key_4, Key_5, Key_6,Key_B, Key_7, Key_8, Key_9, Key_C, Key_Pound, Key_0, Key_Hash, Key_D};

//Scans the Keypad
//Returns what key is pressed in the form of Key enum
enum Keys GetKey( void );
void InitKeypad( void );

//count up and compare keypad inputs to debounce buttons
uint8_t DebouncedKeyInputs(void);

//get keypad input
uint8_t GetKeyPadInputs( void );


#endif


