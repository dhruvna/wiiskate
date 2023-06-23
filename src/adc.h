#ifndef __STM32L476R_NUCLEO_ADC_H
#define __STM32L476R_NUCLEO_ADC_H

#include "stm32l476xx.h"

void delay_ms(volatile uint32_t ms);
void DAC_Init(void);
void DAC_SetValue(uint32_t value);
#endif
