#include "drivers/BNO055.hpp"

/// Creates a new BNO055 Instance.
BNO055::BNO055 (I2C &i2c, uint8_t deviceID) noexcept:
	m_I2C (i2c), m_DeviceID (deviceID)
{}

/// Initialize the BNO055.
void BNO055::Init (void) noexcept {
	// Waits half a second to make sure the BNO055 is ready for
	//  usage.
	Delay::Ms (500);

	// Selects the BNO055 on the I2C bus.
	BNO055::Select ();

	// Reads the CHIP ID's in order to validate
	//  if the chip is connected properly.
	BNO055_ID_t id = BNO055::GetIDs ();
	if (id.gyrID != 0x0F || id.magID != 0x32 || id.accID != 0xFB || id.chipID != 0xA0) {
		printf ("[IMU ID ERR] GYR %02x:%02x MAG %02x:%02x ACC %02x:%02x CHIP %02x:%02x\r\n",
			id.gyrID, 0x0F,
			id.magID, 0x32,
			id.accID, 0xFB,
			id.chipID, 0xA0);

		return;
	}

	// Sets the operational mode to IMU.
	// BNO055::SetOpmode (BNO055_OPMODE_IMU);
}

/// Writes a byte to a BNO055 Register.
void BNO055::WriteRegByte (uint8_t reg, uint8_t byte) noexcept {
	m_I2C.SetMode (I2C_OPMODE_WRITE);

	m_I2C.Start ();
	m_I2C.Write (reg);
	m_I2C.Write (byte);
	m_I2C.Stop ();
}

/// Reads a byte from a BNO055 Register.
uint8_t BNO055::ReadRegByte (uint8_t reg) const noexcept {
	m_I2C.SetMode (I2C_OPMODE_READ);

	m_I2C.Start ();
	m_I2C.Write (reg);

	m_I2C.Start ();
	uint8_t byte = m_I2C.Read ();
	m_I2C.Stop ();

	return byte;
}

/// Sets the operational mode.
void BNO055::SetOpmode (uint8_t mode) noexcept {
	uint8_t reg = BNO055::ReadRegByte (BNO055_P0_OPR_MODE);
	reg &= ~BNO055_OPMODE_Mask;
	reg |= (mode << BNO055_OPMODE_Pos);
	BNO055::WriteRegByte (BNO055_P0_OPR_MODE, reg);
}

/// Selects the device in the I2C peripheral.
void BNO055::Select (void) noexcept {
	m_I2C.SetSlaveAddress (m_DeviceID);
}

/// Gets the device ID's (used for validating connection).
BNO055_ID_t BNO055::GetIDs (void) const noexcept {
	return {
		.gyrID = BNO055::ReadRegByte (BNO055_P0_GYR_ID),
		.magID = BNO055::ReadRegByte (BNO055_P0_MAG_ID),
		.accID = BNO055::ReadRegByte (BNO055_P0_ACC_ID),
		.chipID = BNO055::ReadRegByte (BNO055_P0_CHIP_ID)
	};;
}
