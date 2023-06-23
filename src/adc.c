/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Ethan Nguyen, Dhruv Aggarwal
 * Section: Wednesday 7PM
 * Final Project
 */
#include "adc.h"

void delay_ms(volatile uint32_t ms) {
    // Roughly calibrated for 4 MHz clock frequency
    const uint32_t loop_count = 400; 

    while (ms--) {
        volatile uint32_t i;
        for (i = 0; i < loop_count; i++);
    }
}

void DAC_Init(void) {
	// Enable GPIOA clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// Configure PA4 as Analog mode
	GPIOA->MODER |= GPIO_MODER_MODE4;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD4; // No pull up, no pull down
	// Enable DAC clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;

	DAC->CR &= ~DAC_CR_EN1; // Disable DAC Channel 1 before tampering
	// [DONE] Enable software trigger mode
	DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1;
	// [DONE] Disable trigger
	DAC->CR &= ~DAC_CR_TEN1;
	// [DONE] DAC1 connected in normal mode to external pin only with buffer enabled
	DAC->MCR &= ~DAC_MCR_MODE1;
	
	// [DONE] Enable DAC channel 1
	DAC->CR |= DAC_CR_EN1; 
}

void DAC_SetValue(uint32_t value) {
    if (value > 4095) {
        value = 4095;
    }
    DAC->DHR12R1 &= ~DAC_DHR12R1_DACC1DHR;
		DAC->DHR12R1 = value;
}