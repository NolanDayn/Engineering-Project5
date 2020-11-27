#include "servo.h"

void InitServo(){
		//enable clock on port B
	SET_BITS(RCC->AHBENR, RCC_AHBENR_GPIOBEN);
	
	//set mode to alternate function
	FORCE_BITS(GPIOB->MODER, 3UL << (2*SERVO_PIN), 2UL << (2*SERVO_PIN));

	//set alternate function to AF 6
	FORCE_BITS(GPIOB->AFR[0], 0xF, 6UL);
	
	//no pull up, no pull down
	FORCE_BITS(GPIOB->PUPDR, 3UL << (2*SERVO_PIN), 0);
	
	//enable timer 1 clock
	SET_BITS(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);
	
	
	//up counting
	CLR_BITS(TIM1-> CR1, TIM_CR1_DIR);
	
	//prescaler
	TIM1->PSC = 79;
	
	//auto reload
	TIM1->ARR = 999;
	
	//clear ch 1 output cmp registers
	CLR_BITS(TIM1->CCMR1, TIM_CCMR1_OC1M);
	
	//50% duty cycle
	SetServoDutyCycle(50);
	
	//main output enable
	SET_BITS(TIM1->BDTR, TIM_BDTR_MOE);

	//pwm mode
	SET_BITS(TIM1->CCMR1, TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
	
	//output polarity active high
	CLR_BITS(TIM1 -> CCER, TIM_CCER_CC2NP);
	
	//enable CH2N
	SET_BITS(TIM1 -> CCER, TIM_CCER_CC2NE);
	//enable counter
	SET_BITS(TIM1->CR1, TIM_CR1_CEN);
}

void SetServoDutyCycle(uint32_t dutycycle_percentage){
	TIM1 -> CCR2 = dutycycle_percentage * 10;
}


