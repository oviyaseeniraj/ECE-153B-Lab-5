#include "SysClock.h"

void System_Clock_Init(void) {
	// Enable MSI
	RCC->CR |= RCC_CR_MSION;
	while((RCC->CR & RCC_CR_MSIRDY) == 0);
	
	// Select MSI as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_MSI;
	
	//range of MSI
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= RCC_CR_MSIRANGE_7;
	RCC->CR |= RCC_CR_MSIRGSEL;
	
	//MSI oscillator
	RCC->CR |= RCC_CR_MSIRDY;
	
	//MSI ready
	while((RCC->CR & RCC_CR_MSIRDY) == 0);
}
