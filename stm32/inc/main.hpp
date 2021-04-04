/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#pragma once

/**************************************************
 * Initialization Functions
 **************************************************/

/// Initializes the GY-GPS6MV Peripheral.
void __gy_gps6mv2_init (void) noexcept;

/**************************************************
 * Default Functions
 **************************************************/

/// Enables the used peripheral clocks.
void __setup_rcc (void) noexcept;

/// Handles the setup code.
void __setup (void) noexcept;

/// Gets called forever.
void __loop (void) noexcept;

/// Initializes the FPU.
void __fpu_enable (void) noexcept;

/// Gets called after the startup code is finished.
extern "C" __attribute__ (( noreturn )) int main (void) noexcept;
