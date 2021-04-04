/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#include "hardware/i2c.hpp"

#pragma once

typedef enum {
	TOF10120_DATA_MODE_FILTER = 0,
	TOF10120_DATA_MODE_REAL_TIME = 1	
} TOF10120_Data_Mode_t;

typedef enum {
	TOF10120_DIST_SND_MODE_MODULE_SENDING = 0,
	TOF10120_DIST_SND_MODE_MODULE_HOST_READ = 1
} TOF10120_Distance_Sending_Mode_t;

class TOF10120 {
public:
	TOF10120 (uint8_t sAddr, I2C &i2cPeripheral) noexcept;

	/// Reads the real-time distance in MM.
	uint16_t ReadRealTimeDistanceMM (void) noexcept;

	/// Reads the filtered distance in MM.
	uint16_t ReadFilterDistanceMM (void) noexcept;

	/// Reads the deviation distance in MM (signed).
	int16_t ReadDeviationDistanceMM (void) noexcept;

	/// Reads the distance data mode.
	TOF10120_Data_Mode_t ReadDistanceDataMode (void) noexcept;

	/// Reads the distance sending mode.
	TOF10120_Distance_Sending_Mode_t ReadDistanceSendingMode (void) noexcept;

	/// Reads the maximum measuring distance.
	uint16_t ReadMaxMeasurementDistance (void) noexcept;
private:
	/// Writes a byte to the specified register.
	void Write (uint8_t reg, uint8_t byte) noexcept;

	/// Reads a byte from the specified register.
	uint8_t Read (uint8_t reg) noexcept;
private:
	uint8_t m_SlaveAddress;
	I2C &m_I2CPeripheral;
};
