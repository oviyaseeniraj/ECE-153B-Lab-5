#include "PWM.h"

// [TODO]
void LED_Init() {
	//Enable GPIO Clocks
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // Enable Clock for GPIO Port A (LED)
	
	// Initialize Green LED
	GPIOA->MODER &= ~(GPIO_MODER_MODE5); // Clear MODER5[1:0] for PA5
  GPIOA->MODER |= (GPIO_MODER_MODE5_1);  // Set PA5 as Alternative Function (10 -> 8)
	
	//GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0;

	//Set I/O output speed value as very high speed
	//GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR5;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_0;
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_1;
	
	// GPIO Push-Pull: No pull-up, no pull-down
	//GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5);      // Set PA5 to push-pull
  GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5); // No pull-up, no pull-down
	
}

void PWM_Init() {
	//GPIO Port A Clock
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	//TIM2 Clk
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
	//PA5
	GPIOA->MODER &= ~GPIO_MODER_MODE5_0;
  GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5;
	
	//pwm output ch1 tim2
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;
  GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0;
	
	//chi1 tim2 upwards direction
	TIM2->CR1 &= ~TIM_CR1_DIR;
	
	//prescalar
	TIM2->PSC = 7;
	
	//ARR
	TIM2->ARR = 4095;
	
	//output compare mode
  TIM2->CCMR1 |= TIM_CCMR1_OC1M_1;
  TIM2->CCMR1 |= TIM_CCMR1_OC1M_2;
	
	//preload
  TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
    
	//active high
	TIM2->CCER |= TIM_CCER_CC1P;
  TIM2->CCER |= TIM_CCER_CC1E;
	
	//ccr
  TIM2->CCR1 |= 0;
    
  TIM2->CR1 |= TIM_CR1_CEN;
	
}