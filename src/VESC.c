/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Ethan Nguyen, Dhruv Aggarwal
 * Section: Wednesday 7PM
 * Final Project
 */
#include "VESC.h"

void gradual_increase(uint32_t start_value, uint32_t end_value, uint32_t increment, uint32_t delay_time) {
    for (uint32_t value = start_value; value <= end_value; value += increment) {
        DAC_SetValue(value);
        delay_ms(delay_time);
    }
}

void gradual_decrease(uint32_t start_value, uint32_t end_value, uint32_t increment, uint32_t delay_time) {
    for (uint32_t value = end_value; value >= start_value; value -= increment) {
        DAC_SetValue(value);
        delay_ms(delay_time);
    }
}

void accelerate(void) {
	for (uint32_t value =  1000; value <= 4000; value += 100) {
        DAC_SetValue(value);
    //    delay_ms(100);
    }
}
