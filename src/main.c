/*
 * ECE 153B - Winter 2023
 *
 * Name(s): Dhruv Aggarwal, Ethan D. Nguyen
 * Section: Wednesday 7-9:50PM
 * Lab: Final Project
 */

#include "stm32l476xx.h"
#include "adc.h"
#include "I2C.h"
#include "SysClock.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>
#include "VESC.h"

static int debug = 0;

int main(void) {
	System_Clock_Init(); // Switch System Clock = 16 MHz
	I2C_GPIO_Init();
	I2C_Initialization();
	Init_USARTx(2);
	DAC_Init();
  printf("initialized UART\n");
    
	// nunchuk data
	uint8_t n_init1[2] = {0xF0,0x55};
	uint8_t n_init2[2] = {0xFB,0x00};
	uint8_t n_done[2] = {0x00,0x00};
	uint8_t n_addr_send = 0x52 << 1;
	uint8_t n_addr_receive = 0xA4;
	uint32_t upper_deadband = 2500;
	uint32_t end_value = 2850;
	uint32_t increment = 10;
	uint32_t delay_time = 200; // Adjust this value to control the speed of the ADC value increase
	// initialize nunchuk, problem with sendData, gets stuck after initial send
	I2C_SendData(I2C1, n_addr_send, &n_init1, 2);
	I2C_SendData(I2C1, n_addr_send, &n_init2, 2);
	//data is SDA(PB7), clock is SCL(PB6)
	
	while (1) {
			uint8_t data[6] = {0};
			I2C_ReceiveData(I2C1, n_addr_receive, &data, 6);
			I2C_SendData(I2C1, n_addr_receive, &n_done, 2);
		
			int z = (data[5] >> 0) & 1;
			int c = (data[5] >> 1) & 1;
			if (z == 0){
				printf("Z Button		");
			}
			if (c == 0){
				printf("C Button		");
			}
			if (data[0] > 0) {
					if (data[1] > 200) {
						printf("moved forward\n");
						gradual_increase(upper_deadband, end_value, increment, delay_time);
					  gradual_decrease(upper_deadband, end_value, increment, delay_time); 
					} 
					if (data[1] < 40) {
							printf("moved backward\n");
						delay_ms(500);
						DAC_SetValue(0);
					} else if (data[0] > 200) {
							printf("moved right\n");
					} else if (data[0] < 40) {
							printf("moved left\n");
					} else {
							printf("\n"); 
					}
			}
	} 
	return 0;
}
