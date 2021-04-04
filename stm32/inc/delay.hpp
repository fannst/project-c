/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#pragma once

class Delay {
public:
	
	/// Initializes the delay timer.
	static void Init (void) noexcept;

	/// Delays N US.
	static void Us (uint16_t n) noexcept;

	/// Delays N MS.
	static void Ms (uint16_t n) noexcept;

	/// Delays N S.
	static void S  (uint16_t n) noexcept;
};
