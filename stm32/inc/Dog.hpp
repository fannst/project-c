/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#include "drivers/Servo.hpp"
#include "drivers/BNO055.hpp"

#pragma once

class Dog {
public:
	inline static Dog &getInstance () noexcept {
		return Dog::INSTANCE;
	}

	inline ServoCluster &GetPawFrontLeft () noexcept {
		return m_PawFrontLeft;
	}

	inline ServoCluster &getPawFrontRight () noexcept {
		return m_PawFrontRight;
	}

	inline ServoCluster &getPawBackLeft () noexcept {
		return m_PawBackLeft;
	}

	inline ServoCluster &getPawBackRight () noexcept {
		return m_PawBackRight;
	}

	void Init (void) noexcept;
private:
	Dog (void) noexcept;
	~Dog (void) noexcept = default;

	static Dog INSTANCE;

	ServoCluster	m_PawFrontLeft,
					m_PawFrontRight,
					m_PawBackLeft,
					m_PawBackRight;
};
