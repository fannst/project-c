#include "buzzer.hpp"

#include "delay.hpp"

Buzzer::Buzzer (GPIO_TypeDef *gpio, uint8_t pinOffset) noexcept:
	m_GPIO (gpio), m_PinOffset(pinOffset)
{}

void Buzzer::Init (void) {
	// Makes pin N in the GPIO register General Purpose Output.
	m_GPIO->MODER &= ~(0x3 << (2 * m_PinOffset));
	m_GPIO->MODER |= (0x1 << (2 * m_PinOffset));

	// Makes the pin N default low.
	m_GPIO->ODR &= ~(0x1 << m_PinOffset);
}

void Buzzer::Buzz (uint16_t delay, uint16_t pulses) const {
	for (; pulses > 0; --pulses) {
		m_GPIO->ODR |= (0x1 << m_PinOffset);
		Delay::Us (delay);
		m_GPIO->ODR &= ~(0x1 << m_PinOffset);
		Delay::Us (delay);
	}
}