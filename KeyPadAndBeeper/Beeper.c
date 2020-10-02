
#include "utils.h"
#include "Beeper.h"

#define BEEPER_PIN 6

void InitBeeper( void ){
	
	//enable clock on port B
	SET_BITS(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN);
	
	//set mode to alternate function
	FORCE_BITS(GPIOB->MODER, 3UL << (2*BEEPER_PIN), 2UL << (2*BEEPER_PIN));

	//set alternate function to AF 2
	FORCE_BITS(GPIOB->AFR[0], 0xF << (4 * BEEPER_PIN), 2UL << (4 * BEEPER_PIN));
	
	//no pull up, no pull down
	FORCE_BITS(GPIOB->PUPDR, 3UL << (2*BEEPER_PIN), 0);
	
	//enable timer 4 clock
	SET_BITS(RCC->APB1ENR1, RCC_APB1ENR1_TIM4EN);
	
	//up counting
	CLR_BITS(TIM4-> CR1, TIM_CR1_DIR);
	
	//prescaler
	//TIM4->PSC = prescaleValue; --> 1MHz clock is 1us
	TIM4->PSC = 80-1;
	
	//auto reload
	TIM4->ARR = 5000000 - 1; //auto reload every 500 us 
	
	//clear ch 1 output cmp registers
	CLR_BITS(TIM4->CCMR1, TIM_CCMR1_OC1M);
	
	//Set the duty cycle
	TIM4 -> CCR1 = 0; //Start without beeping
	
	//main output enable
	SET_BITS(TIM4->BDTR, TIM_BDTR_MOE);

	//pwm mode
	SET_BITS(TIM4->CCMR1, TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
	
	//output polarity active high
	CLR_BITS(TIM4 -> CCER, TIM_CCER_CC1P);
	
	//enable CH1
	SET_BITS(TIM4 -> CCER, TIM_CCER_CC1E);
	
	//enable counter
	SET_BITS(TIM4->CR1, TIM_CR1_CEN);

}

void Beep( uint32_t hertz ){
	SET_BITS(TIM4->BDTR, TIM_BDTR_MOE); //Turn beeper on
	SET_BITS(TIM4->CR1, TIM_CR1_CEN);
	
	uint32_t periodInUs = (1.0 / (double)hertz) * 1000000; //get uS period
	uint32_t autoReloadValue = periodInUs * 10 -1; 
	
	TIM4->ARR = autoReloadValue;
	TIM4 -> CCR1 = (TIM4->ARR + 1) / 2; //the capure compare is the period / 2 for a 50% duty cycle
}

void StopBeep(){
	CLR_BITS(TIM4->CR1, TIM_CR1_CEN); //turn beeper off
	CLR_BITS(TIM4->BDTR, TIM_BDTR_MOE);
}

