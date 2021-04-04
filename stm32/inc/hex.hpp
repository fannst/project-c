/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#pragma once

class Hex {
public:
	/// Returns the 4-bit value from a HEX char.
	static uint8_t FromChar (char c) noexcept;
};
