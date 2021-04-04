#include "delay.hpp"

#include "sysclk.hpp"
#include "hardware/wwdg.hpp"

/// Initializes the delay timer.
void Delay::Init (void) noexcept {
	TIM8->ARR = 0;
	TIM8->PSC = __SYSCLOCK_APB2_TIMERS;
}

/// Delays N US.
void Delay::Us (uint16_t n) noexcept {
	Shepherd::Kick ();

	TIM8->CNT = 0;
	TIM8->ARR = n;
	TIM8->CR1 = (TIM_CR1_OPM | TIM_CR1_CEN);

	while (TIM8->CR1 & TIM_CR1_CEN);
}

/// Delays N MS.
void Delay::Ms (uint16_t n) noexcept {
	for (; n > 0; --n) {
		Delay::Us(1000);
	}
}

/// Delays N S.
void Delay::S (uint16_t n) noexcept {
	for (; n > 0; --n) {
		Delay::Ms(1000);
	}
}
