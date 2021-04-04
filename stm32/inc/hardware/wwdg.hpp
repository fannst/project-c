#include "inc.hpp"

#pragma once

class Shepherd {
public:
	/// Initializes the watchdog
	static void Init (void) noexcept;

	/// Kicks the watchdog
	static void Kick (void) noexcept;
};
