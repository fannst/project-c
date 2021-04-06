/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#include "sysclk.hpp"
#include "buzzer.hpp"
#include "delay.hpp"

#include "hardware/wwdg.hpp"
#include "hardware/SPI.hpp"
#include "hardware/usart.hpp"

#include "drivers/GY-GPS6MV2.hpp"
#include "drivers/Servo.hpp"

#pragma once

/**************************************************
 * Classes
 **************************************************/

class Main {
public:
	Main (void) noexcept;

	/// Initializes the GPS hardware.
	void GPSInit (void) noexcept;

	/// Initializes the SPI slave peripheral.
	void SPISlaveInit (void) noexcept;

	/// Enables the used peripheral clocks.
	void SetupRCC (void) noexcept;

	/// Handles the setup code.
	void Setup (void) noexcept;

	/// Gets called forever.
	void Loop (void) noexcept;

	/// Gets the curernt singleton instance.
	static Main &GetInstance() noexcept;

	/// Gets the GPS instance.
	GY_GPS6MV2 &getGPS() noexcept;
private:
	static Main INSTANCE;

	GY_GPS6MV2 m_GPS;		// The GPS module
	Buzzer m_Buzzer;		// The Status Buzzer
};

/**************************************************
 * Default Functions
 **************************************************/

/// Gets called after the startup code is finished.
extern "C" int main (void) noexcept;
