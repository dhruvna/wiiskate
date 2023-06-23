#ifndef __STM32L476R_NUCLEO_VESC_H
#define __STM32L476R_NUCLEO_VESC_H

#include "stm32l476xx.h"
#include "adc.h"

void gradual_increase(uint32_t start_value, uint32_t end_value, uint32_t increment, uint32_t delay_time);
void gradual_decrease(uint32_t start_value, uint32_t end_value, uint32_t increment, uint32_t delay_time);

void accelerate(void);

#endif
