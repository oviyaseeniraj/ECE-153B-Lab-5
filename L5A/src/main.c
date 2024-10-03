/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 5A
 */


#include "ADC.h"
#include "PWM.h"
#include "SysClock.h"
#include "stm32l476xx.h"
#include <stdio.h>

uint32_t data;
volatile float volt;

int main(void) {
    // Initialization
    System_Clock_Init(); // Switch System Clock = 16 MHz

    ADC_Init();

    // [TODO] Initialize PWM
	PWM_Init();
	LED_Init();

    while (1) {
        // [TODO] Trigger ADC and get result
			ADC1->CR |= ADC_CR_ADSTART;
			while((ADC1->ISR & ADC_ISR_EOC) == 0);
			

			
			//get result
			data = ADC1->DR;
			volt = 3.3 * ((4096 - ((float)data)) / 4096);	
			
      // [TODO] LED behavior based on ADC result
			if (data > 4000) {
            TIM2->CCR1 = 4096;
        } else {
            TIM2->CCR1 = data;
        }
        
			//delay
        for (int i = 0; i < 1000; ++i);
    }
}
