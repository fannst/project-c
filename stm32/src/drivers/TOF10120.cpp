#include "drivers/TOF10120.hpp"

TOF10120::TOF10120 (uint8_t sAddr, I2C &i2cPeripheral) noexcept:
	m_SlaveAddress (sAddr), m_I2CPeripheral (i2cPeripheral)
{}

/// Reads the distance data mode.
TOF10120_Data_Mode_t TOF10120::ReadDistanceDataMode (void) noexcept {
	return static_cast<TOF10120_Data_Mode_t> (TOF10120::Read (0x8));
}

/// Reads the distance sending mode.
TOF10120_Distance_Sending_Mode_t TOF10120::ReadDistanceSendingMode (void) noexcept {
	return static_cast<TOF10120_Distance_Sending_Mode_t> (TOF10120::Read (0x9));
}

/// Writes a byte to the specified register.
void TOF10120::Write (uint8_t reg, uint8_t byte) noexcept {
	// Sets the slave address.
	m_I2CPeripheral.SetMode (I2C_OPMODE_WRITE);
	m_I2CPeripheral.SetSlaveAddress (m_SlaveAddress);

	// Writes the slave-address and the register to write to.
	m_I2CPeripheral.Start ();
	m_I2CPeripheral.Write (reg);
	
	// Writes the slave address and register data.
	m_I2CPeripheral.Start ();
	m_I2CPeripheral.Write (byte);

	// Sends the stop condition.
	m_I2CPeripheral.Stop ();
}

/// Reads a byte from the specified register.
uint8_t TOF10120::Read (uint8_t reg) noexcept {
	uint8_t value;

	// Sets the slave address.
	m_I2CPeripheral.SetMode (I2C_OPMODE_READ);
	m_I2CPeripheral.SetSlaveAddress (m_SlaveAddress);

	// Writes the slave-address and the register to write to.
	m_I2CPeripheral.Start ();
	m_I2CPeripheral.Write (reg);
	
	// Writes the slave address, a dummy byte and reads the data.
	m_I2CPeripheral.Start ();
	m_I2CPeripheral.Write (0);
	value = m_I2CPeripheral.Read ();

	// Sends the stop condition.
	m_I2CPeripheral.Stop ();

	return value;
}
