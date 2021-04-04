/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#pragma once

class Buzzer {
public:
	Buzzer (GPIO_TypeDef *gpio, uint8_t pinOffset) noexcept;

	void Init (void);
	void Buzz (uint16_t delay, uint16_t pulses) const;
private:
	GPIO_TypeDef *m_GPIO;
	uint8_t m_PinOffset;
};
