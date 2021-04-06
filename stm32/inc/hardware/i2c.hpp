/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#pragma once

typedef enum {
	I2C_OPMODE_READ = 0,
	I2C_OPMODE_WRITE = 1
} I2C_Operation_Mode_t;

class I2C {
public:

	I2C (I2C_TypeDef *i2c) noexcept;

	/// Initializes the I2C Peripheral.
	void Init (void) noexcept;

	/// Sets the target slave address,
	void SetSlaveAddress (uint8_t sAddr) noexcept;

	/// Generates an start condition.
	void Start (void) noexcept;

	/// Generates an stop condition.
	void Stop (void) noexcept;

	/// Writes the specified byte to the I2C bus.
	void Write (uint8_t byte) noexcept;

	/// Reads one byte from the I2C bus.
	uint8_t Read (void) noexcept;

	/// Sets the I2C Mode.
	void SetMode (I2C_Operation_Mode_t mode) noexcept;
private:
	I2C_TypeDef *m_I2C;
};
