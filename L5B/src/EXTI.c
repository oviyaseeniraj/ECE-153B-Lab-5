#include "EXTI.h"

#include "DAC.h"

void EXTI_Init(void) {
    // [TODO] Configure EXTI for button
		// Initialize User Button
		GPIOC->MODER &= ~(GPIO_MODER_MODE13); // Clear MODER13[1:0] for PC13 to set as input
		GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD13); // No pull-up, no pull-down for PC13
	
		// Configure SYSCFG EXTI
		RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
		SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
		SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	
		// Configure EXTI Trigger
		EXTI->FTSR1 |= EXTI_FTSR1_FT13;
	
		// Enable EXTI
		EXTI->IMR1 |= EXTI_IMR1_IM13;
	
		// Configure and Enable in NVIC
		NVIC_EnableIRQ(EXTI15_10_IRQn);
		NVIC_SetPriority(EXTI15_10_IRQn, 0);
}

#define DAC_MIN 0
#define DAC_MAX 4095
#define DAC_INCREMENT 256

static uint32_t dac_value = 0;
static enum {
    DOWN,
    UP,
} direction = UP;

void EXTI15_10_IRQHandler(void)
	{
    // Clear EXTI13 pending flag
    EXTI->PR1 |= EXTI_PR1_PIF13;

    // Check the direction of the DAC value
    if (direction == UP)
		{
        // Increase the DAC output by DAC_INCREMENT
        dac_value += DAC_INCREMENT;
        
        // Check if DAC value exceeds maximum
        if (dac_value >= DAC_MAX) {
            // Set direction to DOWN
            direction = DOWN;
            // Clamp the value to DAC_MAX
            dac_value = DAC_MAX;
        }
    } else {
        // Decrease the DAC output by DAC_INCREMENT
        if (dac_value >= DAC_INCREMENT) {
            dac_value -= DAC_INCREMENT;
        } else {
            // Set direction to UP
            direction = UP;
            // Set DAC value to minimum
            dac_value = DAC_MIN;
        }
    }

    // Write new DAC value
    DAC_Write_Value(dac_value);
	}
