#include "sysclk.hpp"

void SysClk::Init (void) {
	// Enables the HSE clock.
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

	// Enables FLASH PRFT, and sets the latency to 7WS.
	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_7WS;

	// Configures the HPRE, PPRE1 and PPRE2 bits.
	RCC->CFGR = ((0b100 << RCC_CFGR_PPRE2_Pos)
			| (0b101 << RCC_CFGR_PPRE1_Pos)
			| (0b0000 << RCC_CFGR_HPRE_Pos));

	// Configures the main PLL
	RCC->PLLCFGR = ((9 << RCC_PLLCFGR_PLLQ_Pos)
			| (0 << RCC_PLLCFGR_PLLP_Pos)
			| (216 << RCC_PLLCFGR_PLLN_Pos)
			| (4 << RCC_PLLCFGR_PLLM_Pos)
			| (2 << RCC_PLLCFGR_PLLR_Pos)
			| RCC_PLLCFGR_PLLSRC_HSE);

	// Enables the PLL and waits for it to become ready.
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));

	// Use the PLL as main clock source.
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SW) != RCC_CFGR_SW_PLL);
}