/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Ethan Nguyen, Dhruv Aggarwal
 * Section: Wednesday 7PM
 * Lab: 4A
 */
#include "UART.h"

void UART1_Init(void) {
	RCC->APB2ENR|=RCC_APB2ENR_USART1EN;
	RCC->CCIPR &= ~RCC_CCIPR_USART1SEL;
	RCC->CCIPR|=RCC_CCIPR_USART1SEL_0;//select system clock as the USART2 clock source
	
}

void UART2_Init(void) {
	RCC->APB1ENR1|=RCC_APB1ENR1_USART2EN;//enables the clock in peripheral clock reg
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;
	RCC->CCIPR|= RCC_CCIPR_USART2SEL_0;//select system clock as the USART2 clock source in the peripheral independent clock config register
}

void UART1_GPIO_Init(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->MODER &= ~(GPIO_MODER_MODE6);
	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	GPIOB->MODER &= ~(GPIO_MODER_MODE7);
	GPIOB->MODER |= GPIO_MODER_MODE7_1;
	
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6);//set AFSEL3 to AF7 (0111)
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_0;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_1;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_2;
	
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL7);//set AFSEL2 to AF7 (0111)
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_0;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_1;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_2;    
	
	GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED6;//clear bits
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6; //set to very high speed PB6 (11) 
	GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED7;//clear bits
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED7; //set to very high speed PB7 (11) 
	
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT6;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT7; 
	
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0; //set pull up resistor for PB6
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD7_0; //set pull up resistor PB7
}

void UART2_GPIO_Init(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	GPIOA->MODER &= ~(GPIO_MODER_MODE2);
	GPIOA->MODER |= GPIO_MODER_MODE2_1;
	GPIOA->MODER &= ~(GPIO_MODER_MODE3);
	GPIOA->MODER |= GPIO_MODER_MODE3_1;
	
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2|GPIO_AFRL_AFSEL3);//set AFSEL3 to AF7 (0111)
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_2;
	
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0;//set AFSEL2 to AF7 (0111)
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_1;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_2; 
	
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED2;//clear bits
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2; //set to very high speed PB6 (11) 
	GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED3;//clear bits
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED3; //set to very high speed PB7 (11) 
	
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;
	
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0; //set pull up resistor for PB6
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD3_0; //set pull up no PB7
}

void USART_Init(USART_TypeDef* USARTx) {
	USARTx->CR1 &= ~USART_CR1_UE; //disable USART in order to tamper with settings
	
	USARTx->CR1 &= ~USART_CR1_M; //set to 8-bit character length

	USARTx->CR1 &= ~USART_CR1_OVER8; //oversampling by 16
	
  USARTx->BRR |= 8333; //set BRR to 8333 for baudrate of 9600
	
	USARTx->CR1 |= USART_CR1_TE; //enable transmitter
	USARTx->CR1 |= USART_CR1_RE; //enable receiver
	
	USARTx->CR1 |= USART_CR1_UE; //enable the control registers
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE)); //wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}

void Init_USARTx(int x) {
	if(x == 1) {
		UART1_Init();
		UART1_GPIO_Init();
		USART_Init(USART1);
	} else if(x == 2) {
		UART2_Init();
		UART2_GPIO_Init();
		USART_Init(USART2);
	} else {
		// Do nothing...
	}
}