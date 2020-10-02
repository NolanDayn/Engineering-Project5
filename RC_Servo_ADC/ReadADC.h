#ifndef __READ_ADC__H
#define __READ_ADC__H

#include "stm32l476xx.h"

void TurnOn_ADC1(void);
void TurnOnADC_CLK(void);
void PA1_AnalogConfig(void);
void Initialize_ADC1(void);
uint16_t TriggerADCConversion(void);

#endif

