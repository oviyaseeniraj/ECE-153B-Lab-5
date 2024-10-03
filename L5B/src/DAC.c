#include "stm32l476xx.h"

static void DAC_Pin_Init(void) {
	//set pa4 to no pull up no pull down
	
	//activate clock
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  
	//set to analog
	GPIOA->MODER &= ~(GPIO_MODER_MODE4); // Clear MODER5[1:0] for PA4
  GPIOA->MODER |= (GPIO_MODER_MODE4_0);
  GPIOA->MODER |= (GPIO_MODER_MODE4_1);
	
	//set to push pull
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT4);
	
	//set to no push no pull
  GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD4);
}

void DAC_Write_Value(uint32_t value) {
    // [TODO] Write DAC1Ch1 right-aligned 12-bit value
		DAC1->DHR12R1 = value & 0x0FFF; // mask to ensure only 12 bits are considered
}

void DAC_Init(void) {
    DAC_Pin_Init();

    // [TODO] Enable DAC clock
		RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;

    // [TODO] Enable software trigger mode
		DAC1->CR |= DAC_CR_TEN1;

    // [TODO] Disable trigger
		DAC1->CR &= ~DAC_CR_TSEL1;

    // [TODO] DAC1 connected in normal mode to external pin only with buffer enabled
		DAC1->MCR &= ~DAC_MCR_MODE1;
    DAC1->MCR |= DAC_MCR_MODE1_1; // Buffer enabled

    // [TODO] Enable DAC channel 1
		DAC1->CR |= DAC_CR_EN1;

    DAC_Write_Value(0);
}
