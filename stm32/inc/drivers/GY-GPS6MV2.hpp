/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#include "hardware/usart.hpp"

#pragma once

/**************************************************
 * Global Device Configuration
 **************************************************/

#define GY_GPS6MV2_BAUD					(9600)
#define GY_GPS6MV2_BUFFER_SIZE			(128)

/**************************************************
 * Data Types
 **************************************************/

/*
	<Driver Creation Steps>
	1. Call the static interrupt handler for the driver, 
		with class instance passed.
	2. Enable USART peripheral in RCC.
	3. Enable the interrupt in the NVIC.
	4. Call init on the driver.
	5. Clear Line Buffer
*/

typedef struct {
	uint8_t		h,		/* Hours */
				m,		/* Minutes */
				s;		/* Seconds */
} GY_GPS6MV2_UTC_Pos_t;

typedef enum {
	FIX_NOT_AVAILABLE_OR_INVALID = 0,
	GPS_SPS_MODE__FIX_VALID = 1,
	DIFF_GPS__SPS_MODE__FIX_VALID = 2,
	GPS_PPS_MODE__FIX_VALID = 3
} GY_GPS6MV2_PositionFix_t;

typedef enum {
	UNIT_STEP_MM = 0, UNIT_STEP_CM = 1, UNIT_STEP_M = 2,
	UNIT_STEP_KM = 3, UNIT_STEP_INVALID = 4
} GY_GPS6MV2_UnitStep_t;

typedef enum {
	GGA = 0, GLL = 1, GSA = 2, GSV = 3,
	RMC = 4, VTG = 5, ZDA = 6, INVALID = 7
} GY_GPS6MV2_Record_t;

typedef enum {
	GLL_FIX_MODE_T_AUTONOMOUS = 0,
	GLL_FIX_MODE_T_NO_FIX = 1,
	GLL_FIX_MODE_T_DGPS = 2,
	GLL_FIX_MODE_T_DR = 3,
	GLL_FIX_MODE_T_INVALID = 4
} GY_GPS6MV2_GLL_FixMode_t;

typedef struct {
	/* Lat / Long */
	float						lat;
	float						lon;
	/* Direction */
	unsigned 					ns : 1;			/* 0: North, 1: South */
	unsigned 					ew : 1;			/* 0: East,  1: West */
	/* Altitude */
	float						altitude;
	GY_GPS6MV2_UnitStep_t		altiStep;
	/* Geoidal */
	float						geoidal;
	GY_GPS6MV2_UnitStep_t		geoidalStep;
	/* Other things */
	GY_GPS6MV2_UTC_Pos_t		utcPos;			/* The UTC Offset / Position */
	float						hdop;
	/* Satelite / Technical Info */
	GY_GPS6MV2_PositionFix_t 	pf;				/* Position Fix */
	uint8_t 					nSatelites;		/* Number of satelites used */
} GY_GPS6MV2_GGA_Data_t;

typedef struct {
	/* Lat / Long */
	float						lat;
	float						lon;
	/* Direction */
	unsigned 					ns : 1;			/* 0: North, 1: South */
	unsigned 					ew : 1;			/* 0: East,  1: West */
	/* Other things */
	GY_GPS6MV2_UTC_Pos_t		utcPos;			/* The UTC Offset / Position */
	unsigned					iv : 1;			/* 0: Valid, 1: Invalid */
	GY_GPS6MV2_GLL_FixMode_t	fm;				/* Fix Mode (A=autonomous, N = No fix, D=DGPS, E=DR) */
} GY_GPS6MV2_GLL_t;

/**************************************************
 * Functions
 **************************************************/

void parseGY_GPSMV2_GGA_Record (char *str, GY_GPS6MV2_GGA_Data_t &ret) noexcept;

void parseGY_GPSMV2_GLL_Record (char *str, GY_GPS6MV2_GLL_t &ret) noexcept;

GY_GPS6MV2_GLL_FixMode_t parseGY_GPSMV2_GLL_FixMode (const char *str) noexcept;

GY_GPS6MV2_UnitStep_t parseGY_GPS6MV2_UnitStep (const char *str);

GY_GPS6MV2_Record_t parseGY_GPS6MV2_RecordType (const char *str);

/**************************************************
 * Classes
 **************************************************/

class GY_GPS6MV2 : public USART {
public:
	/// Creates a new GY GPS6MV2 Driver instance.
	GY_GPS6MV2 (USART_TypeDef *usart) noexcept;

	/// Performs the driver initialization procedure.
	void Init (uint32_t clk) noexcept;

	/// Gets called when a new char is received by the USART interrupt.
	void HandleISR (char c) noexcept;

	/// Gets called once a new record is available to read.
	void HandleRecord (void) noexcept;

	/// gets the GGA record data.
	const GY_GPS6MV2_GGA_Data_t &getGGA (void) const noexcept;

	/// Gets the GGL record data.
	const GY_GPS6MV2_GLL_t &getGLL (void) const noexcept;

	/// Calculates the NMEA 0183 Checksum
	static uint8_t CalculateChecksum (const char *s);

	/// Parses the checksum from NMEA 0183 packet.
	static uint8_t ParseChecksum (const char *cs);
private:
	char m_LineBuffer[GY_GPS6MV2_BUFFER_SIZE];
	uint16_t m_LineBufferLevel;

	GY_GPS6MV2_GGA_Data_t m_GGA;
	GY_GPS6MV2_GLL_t m_GLL;
};
