/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#include "delay.hpp"

#include "hardware/i2c.hpp"

#pragma once

/**************************************************
 * Interrupt Service Routines
 **************************************************/

/**************************************************
 * Registers (PAGE 0)
 **************************************************/

/* Magnetometer Radius */

#define BNO055_P0_MAG_RADIUS_MSB	0x6A
#define BNO055_P0_MAG_RADIUS_LSB	0x69

/* Accelerometer Radius*/

#define BNO055_P0_ACC_RADIUS_MSB	0x68
#define BNO055_P0_ACC_RADIUS_LSB	0x67

/* Gyro Offset */

#define BNO055_P0_GYR_OFFSET_Z_MSB	0x66
#define BNO055_P0_GYR_OFFSET_Z_LSB	0x65

#define BNO055_P0_GYR_OFFSET_Y_MSB	0x64
#define BNO055_P0_GYR_OFFSET_Y_LSB	0x63

#define BNO055_P0_GYR_OFFSET_X_MSB	0x62
#define BNO055_P0_GYR_OFFSET_X_LSB	0x61

/* Mag Offset */

#define BNO055_P0_MAG_OFFSET_Z_MSB	0x60
#define BNO055_P0_MAG_OFFSET_Z_LSB	0x5F

#define BNO055_P0_MAG_OFFSET_Y_MSB	0x5E
#define BNO055_P0_MAG_OFFSET_Y_LSB	0x5D

#define BNO055_P0_MAG_OFFSET_X_MSB	0x5C
#define BNO055_P0_MAG_OFFSET_X_LSB	0x5B

/* Accelerometer Offset */

#define BNO055_P0_ACC_OFFSET_Z_MSB	0x5A
#define BNO055_P0_ACC_OFFSET_Z_LSB	0x59

#define BNO055_P0_ACC_OFFSET_Y_MSB	0x58
#define BNO055_P0_ACC_OFFSET_Y_LSB	0x57

#define BNO055_P0_ACC_OFFSET_X_MSB	0x56
#define BNO055_P0_ACC_OFFSET_X_LSB	0x55

/* System Stuff */

#define BNO055_P0_AXIS_MAP_SIGN		0x42
#define BNO055_P0_AXIS_MAP_CONFIG	0x41
#define BNO055_P0_TEMP_SOURCE		0x40
#define BNO055_P0_SYS_TRIGGER		0x3F
#define BNO055_P0_PWR_MODE			0x3E
#define BNO055_P0_OPR_MODE			0x3D
#define BNO055_P0_UNITSEL			0x3B
#define BNO055_P0_SYS_ERR			0x3A
#define BNO055_P0_SYS_STATUS		0x39
#define BNO055_P0_SYS_CLK_STATUS	0x38
#define BNO055_P0_INT_STA			0x37
#define BNO055_P0_ST_STATUS			0x36
#define BNO055_P0_CALIB_STAT		0x35

/* Temparature Data */

#define BNO055_P0_TEMP				0x34

/* Gravity Data */

#define BNO055_P0_GRV_Data_Z_MSB	0x33
#define BNO055_P0_GRV_Data_Z_LSB	0x32

#define BNO055_P0_GRV_Data_Y_MSB	0x31
#define BNO055_P0_GRV_Data_Y_LSB	0x30

#define BNO055_P0_GRV_Data_X_MSB	0x2F
#define BNO055_P0_GRV_Data_X_LSB	0x2E

/* Linear Acceleration Data */

#define BNO055_P0_LIA_Data_Z_MSB	0x2D
#define BNO055_P0_LIA_Data_Z_LSB	0x2C

#define BNO055_P0_LIA_Data_Y_MSB	0x2B
#define BNO055_P0_LIA_Data_Y_LSB	0x2A

#define BNO055_P0_LIA_Data_X_MSB	0x29
#define BNO055_P0_LIA_Data_X_LSB	0x28

/* Pitch Data */

#define BNO055_P0_Pitch_MSB			0x1F
#define BNO055_P0_Pitch_LSB			0x1E

/* Roll Data */

#define BNO055_P0_Roll_MSB			0x1D
#define BNO055_P0_Roll_LSB			0x1C

/* Heading Data */

#define BNO055_P0_Heading_MSB		0x1B
#define BNO055_P0_Heading_LSB		0x1A

/* Gyroscope Data */

#define BNO055_P0_GYR_DATA_Z_MSB	0x19
#define BNO055_P0_GYR_DATA_Z_LSB	0x18

#define BNO055_P0_GYR_DATA_Y_MSB	0x17
#define BNO055_P0_GYR_DATA_Y_LSB	0x16

#define BNO055_P0_GYR_DATA_X_MSB	0x15
#define BNO055_P0_GYR_DATA_X_LSB	0x14

/* Magnetometer Data */

#define BNO055_P0_MAG_DATA_Z_MSB	0x13
#define BNO055_P0_MAG_DATA_Z_LSB	0x12

#define BNO055_P0_MAG_DATA_Y_MSB	0x11
#define BNO055_P0_MAG_DATA_Y_LSB	0x10

#define BNO055_P0_MAG_DATA_X_MSB	0xF
#define BNO055_P0_MAG_DATA_X_LSB	0xE

/* Acceleration Data */

#define BNO055_P0_ACC_DATA_Z_MSB	0xD
#define BNO055_P0_ACC_DATA_Z_LSB	0xC

#define BNO055_P0_ACC_DATA_Y_MSB	0xB
#define BNO055_P0_ACC_DATA_Y_LSB	0xA

#define BNO055_P0_ACC_DATA_X_MSB	0x9
#define BNO055_P0_ACC_DATA_X_LSB	0x8

/* Device Info */

#define BNO055_P0_PAGE_ID			0x7
#define BNO055_P0_BL_Rev_ID_VER		0x6
#define BNO055_P0_SW_REV_ID_MSB		0x5
#define BNO055_P0_SW_REV_ID_LSB		0x4
#define BNO055_P0_GYR_ID			0x3
#define BNO055_P0_MAG_ID			0x2
#define BNO055_P0_ACC_ID			0x1
#define BNO055_P0_CHIP_ID			0x0

/**************************************************
 * Registers (PAGE 1)
 **************************************************/

/**************************************************
 * Register Bits
 **************************************************/

/* Opmode register values */

#define BNO055_OPMODE_Pos			(0)
#define BNO055_OPMODE_Mask			(0b00001111 << BNO055_OPMODE_Pos)
#define BNO055_OPMODE_CONFIG 		(0b00000000 << BNO055_OPMODE_Pos)
#define BNO055_OPMODE_ACCONLY 		(0b00000001 << BNO055_OPMODE_Pos)
#define BNO055_OPMODE_MAGONLY 		(0b00000010 << BNO055_OPMODE_Pos)
#define BNO055_OPMODE_GYROONLY 		(0b00000011 << BNO055_OPMODE_Pos)
#define BNO055_OPMODE_ACCMAG 		(0b00000100 << BNO055_OPMODE_Pos)
#define BNO055_OPMODE_ACCGYRO 		(0b00000101 << BNO055_OPMODE_Pos)
#define BNO055_OPMODE_MAGGYRO 		(0b00000110 << BNO055_OPMODE_Pos)
#define BNO055_OPMODE_AMG 			(0b00000111 << BNO055_OPMODE_Pos)
#define BNO055_OPMODE_IMU 			(0b00001000 << BNO055_OPMODE_Pos)
#define BNO055_OPMODE_COMPASS 		(0b00001001 << BNO055_OPMODE_Pos)
#define BNO055_OPMODE_M4G 			(0b00001010 << BNO055_OPMODE_Pos)
#define BNO055_OPMODE_NDOF_FMC_OFF 	(0b00001011 << BNO055_OPMODE_Pos)
#define BNO055_OPMODE_NDOF 			(0b00001100 << BNO055_OPMODE_Pos)

/* Acceleration GRange Position */

#define BNO055_ACC_GRANGE_Pos		(0)
#define BNO055_ACC_GRANGE_2G 		(0b00000000 << BNO055_ACC_GRANGE_Pos)
#define BNO055_ACC_GRANGE_4G 		(0b00000001 << BNO055_ACC_GRANGE_Pos)
#define BNO055_ACC_GRANGE_8G 		(0b00000010 << BNO055_ACC_GRANGE_Pos)
#define BNO055_ACC_GRANGE_16G 		(0b00000011 << BNO055_ACC_GRANGE_Pos)

/* Acceleration, Linear m/s Acceleration, Gravity mg vector */
#define BNO055_UNITSEL_ALAG_Pos		(0)
#define BNO055_UNITSEL_ALAG_MS2		(0b00000000 << BNO055_UNITSEL_ALAG_Pos)
#define BNO055_UNITSEL_ALAG_MG		(0b00000001 << BNO055_UNITSEL_ALAG_Pos)
/* Angular Rate */
#define BNO055_UNITSEL_AR_DPS_Pos	(1)
#define BNO055_UNITSEL_AR_DPS		(0b00000000 << BNO055_UNITSEL_AR_DPS_Pos)
#define BNO055_UNITSEL_AR_RPS		(0b00000001 << BNO055_UNITSEL_AR_DPS_Pos)
/* Euler Angles */
#define BNO055_UNITSEL_EA_Pos		(2)
#define BNO055_UNITSEL_EA_DEGREES	(0b00000000 << BNO055_UNITSEL_EA_Pos)
#define BNO055_UNITSEL_EA_RADIANS	(0b00000001 << BNO055_UNITSEL_EA_Pos)
/* Temperature */
#define BNO055_UNITSEL_TEMP_Pos		(4)
#define BNO055_UNITSEL_TEMP_CELCIUS	(0b00000000 << BNO055_UNITSEL_TEMP_Pos)
#define BNO055_UNITSEL_TEMP_FAHRENH	(0b00000001 << BNO055_UNITSEL_TEMP_Pos)
/* Data Output Format */
#define BNO055_UNITSEL_DO_Pos		(7)
#define BNO055_UNITSEL_DO_WINDOWS	(0b00000000 << BNO055_UNITSEL_DO_Pos)
#define BNO055_UNITSEL_DO_ANDROID	(0b00000001 << BNO055_UNITSEL_DO_Pos)

/**************************************************
 * Config
 **************************************************/

#define BNO055_DEFAULT_I2C_ID		(0x29)
#define BNO055_ALTERNATIVE_I2C_ID	(0x28)

/**************************************************
 * Classes
 **************************************************/

typedef struct {
	uint8_t gyrID;
	uint8_t magID;
	uint8_t accID;
	uint8_t chipID;
} BNO055_ID_t;

class BNO055 {
public:
	/// Creates a new BNO055 Instance.
	BNO055 (I2C &i2c, uint8_t deviceID) noexcept;

	/// Destroys the current BNO055 Instance.
	~BNO055 (void) noexcept = default;

	/// Initialize the BNO055.
	void Init (void) noexcept;

	/// Writes a byte to a BNO055 Register.
	void WriteRegByte (uint8_t reg, uint8_t byte) noexcept;

	/// Reads a byte from a BNO055 Register.
	uint8_t ReadRegByte (uint8_t reg) const noexcept;

	/// Gyroscope Data Methods.
	uint16_t ReadGyroX (void) const noexcept;
	uint16_t ReadGyroY (void) const noexcept;
	uint16_t ReadGyroZ (void) const noexcept;

	/// Gravity Data Method.
	uint16_t ReadGravX (void) const noexcept;
	uint16_t ReadGravY (void) const noexcept;
	uint16_t ReadGravZ (void) const noexcept;

	/// Sets the operational mode.
	void SetOpmode (uint8_t mode) noexcept;

	/// Selects the device in the I2C peripheral.
	void Select (void) noexcept;

	/// Gets the device ID's (used for validating connection).
	BNO055_ID_t GetIDs (void) const noexcept;
private:
	uint8_t m_DeviceID;
	I2C &m_I2C;
};
