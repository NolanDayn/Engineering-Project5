#include "stm32l476xx.h"


void ADC1_Wakeup(void){
	
	int wait_time;
	
	//To start ADC operations, the following sequence should be applied
	//DEEPPWD = 0: ADC not in deep power down
	//DEEPPWD = 1: ADC in deep-power-down (default reset state)
	if((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD){
		//exit deep power down mode if still in that state
		ADC1->CR &= ~ADC_CR_DEEPPWD;
	}
	
	//Enable the ADC internal voltage regulator
	//Before performing any operations such as launching a calibratio or
	//enabling the ADC, the ADC voltage regulator must first be eneabled and the
	//software must wait for the regulator start-up time
	ADC1->CR |= ADC_CR_ADVREGEN;
	
	//Wait for the ADC voltage regulator start-up time. the software must wait for
	//the startup time of the ADC voltage regulator (T_ADCVREG_STUP, ie 20us) before 
	//launching a calibration or enabling the ADC
	wait_time = 20*(80000000 / 1000000);
	while(wait_time != 0){
		wait_time--;
	}
	
}

void PA1_AnalogConfig(void){

	//Enable the clock of GPIO A
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOAEN;
	
	//GPIO Mode
	//00 = Digital Input, 01 = Digital Output
	//10 = Alternate function, 11 = Analog(default)
	GPIOA->MODER |= 3U<<2; //Config PA1 as analog mode
	
	//GPIO Push-Pull:
	// 00 = No pull-up/pull-down, 01 = Pull-up
	// 10 = pull-down, 11 = reserved
	GPIOA->PUPDR &= ~(3U<<2); //No pull up, no pull down
	
	//GPIO port analog switch control register ASCR
	// 0 = Disconnect analog switch to the ADC input (reset state)
	// 1 = Connect analog switch to the ADC input
	GPIOA->ASCR |= 1U<<1; //pin 1
}

void TurnOnADC_CLK(void){
	RCC->CR |= RCC_CR_HSION; //Turn on Internal High Speed oscillator (HSI16)
	
	//wait for HSI to be ready
	while((RCC->CR &= RCC_CR_HSIRDY) == 0){
	}
}

void Initialize_ADC1(void){
	
	//Enable ADC clock bit (16MHz clock always used foor ADC on STM32L
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	
	//Disable ADC1 control
	ADC1->CR &= ~ADC_CR_ADEN;
	
	//Enable I/O analog switches voltage booster
	ADC123_COMMON->CCR |= SYSCFG_CFGR1_BOOSTEN;

	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;
	
	//!!!!!!!!!SET PRESCALER (select clock not divided)
	ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;
	
	//Select synchronous clock mode
  ADC123_COMMON->CCR |= ADC_CCR_CKMODE_1;
	
	//Configure all ADCs as indpendent
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;
	
	//Wake up the ADC
	ADC1_Wakeup();
	
	//Configure RES BIT 00 = 12 bit, 01 = 10 bit
	//10 = 8 bit, 11 = 6 bit
	ADC1->CFGR &= ~ADC_CFGR_RES; //we set the resolution to 12 bits
	
	ADC1->CFGR &= ~ADC_CFGR_ALIGN; //based on online reading we assume right is 0, 1 is left
	
	//Clear ADC_SQR1_L in ADC1->SQR1 to select 1 conversion in the regular channel conversion sequence
	ADC1->SQR1 &= ~ADC_SQR1_L;
	
	//Channel 6 as first conversion of regular sequence
	ADC1->SQR1 |= (ADC_SQR1_SQ1_1 | ADC_SQR1_SQ1_2);
	
	//Configure channel 6 as single ended
	ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_6;
	
	//set sampling time to 5 clk cycles
	ADC1->SMPR1 |= (ADC_SMPR1_SMP6_0 | ADC_SMPR1_SMP6_2);
	
	//set adc as discontinuous
	ADC1->CFGR &= ~ADC_CFGR_CONT; 
	
	//select software trigger
	ADC1->CFGR &= ~ADC_CFGR_EXTEN;
	
	//enable ADC1
	ADC1->CR |= ADC_CR_ADEN;
	
	//wait until ADC1 is ready
	while((ADC1->ISR &= ADC_ISR_ADRDY) == 0){
	}
}

uint16_t TriggerADCConversion(void){

	uint16_t adcVal;
	//start ADC conversion
	ADC1->CR |= ADC_CR_ADSTART;
	
	//wait for ADC conversion to complete
	while((ADC123_COMMON->CSR &= ADC_CSR_EOC_MST) == 0){
	}
	
	adcVal = ADC1->DR;
	
	return adcVal;
}

