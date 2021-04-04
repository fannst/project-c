/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#pragma once

class ServoCluster {
public:

	/// Creates a new servo cluster instance.
	ServoCluster (TIM_TypeDef *tim, float degrees, float min, float max) noexcept;

	/// Initializes the timer to serve the servos.
	void Init (uint16_t presc, uint16_t arr) noexcept;

	/// Moves servo N to specified amount of degrees.
	void MoveServo (uint8_t n, float deg) noexcept;

private:
	TIM_TypeDef *m_TIM;
	float m_Degrees, m_Min, m_Max;
};
