#include "ADC.h"

#include "stm32l476xx.h"

#include <stdint.h>

void ADC_Wakeup(void) {
    int wait_time;

    // To start ADC operations, the following sequence should be applied
    // DEEPPWD = 0: ADC not in deep-power down
    // DEEPPWD = 1: ADC in deep-power-down (default reset state)
    if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD)
        ADC1->CR &= ~ADC_CR_DEEPPWD; // Exit deep power down mode if still in that state

    // Enable the ADC internal voltage regulator
    // Before performing any operation such as launching a calibration or enabling the ADC, the ADC
    // voltage regulator must first be enabled and the software must wait for the regulator start-up
    // time.
    ADC1->CR |= ADC_CR_ADVREGEN;

    // Wait for ADC voltage regulator start-up time
    // The software must wait for the startup time of the ADC voltage regulator (T_ADCVREG_STUP)
    // before launching a calibration or enabling the ADC.
    // T_ADCVREG_STUP = 20 us
    wait_time = 20 * (80000000 / 1000000);
    while (wait_time != 0) {
        wait_time--;
    }
}

void ADC_Common_Configuration() {
    // [TODO]
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	// switch voltage booster
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;
	
	//V refint
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;
	
	//clk not divided
	ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;
	
	//HCLK/1 sychronous clk mode
	ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;
	
	//independent mode
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;
	
	
	
	
}

void ADC_Pin_Init(void) {
    // [TODO]
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	//analog mode
	GPIOA->MODER &= ~GPIO_MODER_MODE1;
	GPIOA->MODER |= GPIO_MODER_MODE1;
	
	//no pull-up, no pull-down
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;
	
	//channel 6
	GPIOA->ASCR |= GPIO_ASCR_ASC1;
	
	
}

void ADC_Init(void) {
    // [TODO] Enable & Reset ADC Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	RCC->AHB2RSTR |= RCC_AHB2RSTR_ADCRST;
	RCC->AHB2RSTR &= ~RCC_AHB2RSTR_ADCRST;

    // Other ADC Initialization
    ADC_Pin_Init();
    ADC_Common_Configuration();
    ADC_Wakeup();

    // [TODO] Other Configuration
	//disable ADC
	ADC1->CR |= ADC_CR_ADEN;
	ADC1->CR |= ADC_CR_ADDIS;
	
	// 12 bit resolution 
	ADC1->CFGR &= ~ADC_CFGR_RES;
	
	//right alignment
	ADC1->CFGR &= ~ADC_CFGR_ALIGN;
	
	//sequence length = 1
	ADC1->SQR1 &= ~ADC_SQR1_L;
	ADC1->SQR1 |= (ADC_SQR1_SQ1_1 | ADC_SQR1_SQ1_2);
	
	//channel 6 single ended Mode
	ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_6;
	
	//sampling time = 24.5, ADC clock cycles SMP = 011
	ADC1->SMPR1 &= ~ADC_SMPR1_SMP6;
	ADC1->SMPR1 |= (ADC_SMPR1_SMP6_0 | ADC_SMPR1_SMP6_1);
	
	//single conversion mode
	ADC1->CFGR &= ~ADC_CFGR_CONT;
	
	//disable hardware trigger detection
	ADC1->CFGR &= ~ADC_CFGR_EXTEN;
	
	ADC1->CR |= ADC_CR_ADEN;
	
	//ready ADC flag
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0);
	
	
}
