#include "drivers/GY-GPS6MV2.hpp"

#include "hex.hpp"

/**************************************************
 * Functions
 **************************************************/

void parseGY_GPSMV2_GGA_Record (char *str, GY_GPS6MV2_GGA_Data_t &ret) noexcept {
	char *tokenStart = str + 5;
	uint8_t i = 0;

	while (true) {
		// Finds the next token in the string, if this fails it will return null
		//  and we will return from the loop. If there is a token found replace it
		//  with '\0' to create compatibility with string functions.

		char *tokenEnd = strchr(tokenStart, ',');
		if (tokenEnd == NULL) {
			break;
		}

		tokenEnd[0] = '\0';

		// Checks if the current token is empty, if so we want to keep
		//  the original value so we skip the loop.
		
		if (tokenStart[0] != '\0') {
			switch (i) {
			case 0: // Label
			     break;
			case 1: // UTC Position
				break;
			case 2: // Latitude
				ret.lat = atof (tokenStart);
				break;
			case 3: // N/S Indicator
				ret.ns = (memcmp (tokenStart, "S", 1) == 0);
				break;
			case 4: // Longitude
				ret.lon = atof (tokenStart);
				break;
			case 5: // E/W Indicator
				ret.ew = (memcmp (tokenStart, "W", 1) == 0);
				break;
			case 6: // Position Fix
				ret.pf = static_cast<GY_GPS6MV2_PositionFix_t> (atoi (tokenStart));
				break;
			case 7: // N of Satelites
				ret.nSatelites = atoi (tokenStart);
				break;
			case 8: // Idk... HDOP ?
				ret.hdop = atof (tokenStart);
				break;
			case 9: // Altitude
				ret.altitude = atof (tokenStart);
				break;
			case 10: // Altitude Step
				ret.altiStep = parseGY_GPS6MV2_UnitStep (tokenStart);
				break;
			case 11: // Geoidal
				ret.geoidal = atof (tokenStart);
				break;
			case 12: // Geoidal step
				ret.geoidalStep = parseGY_GPS6MV2_UnitStep (tokenStart);
				break;
			default:
				break;
			}
		}

		// Since we already know if the current token contains a comma or not, and we
		//  want to parse it, we will set a null term at the next comma position.
		
		tokenStart = tokenEnd + 1;

		++i;
	}
}

void parseGY_GPSMV2_GLL_Record (char *str, GY_GPS6MV2_GLL_t &ret) noexcept {
	char *tokenStart = str + 5;
	uint8_t i = 0;

	while (true) {
		char *tokenEnd = strchr(tokenStart, ',');
		if (tokenEnd == NULL) {
			break;
		}

		tokenEnd[0] = '\0';

		if (tokenStart[0] != '\0') {
			switch (i) {
			case 0: // Message ID
				break;
			case 1: // Latitude
				ret.lat = atof (tokenStart);
				break;
			case 2: // N/S Indicator
				ret.ns = (memcmp (tokenStart, "S", 1) == 0);
				break;
			case 3: // Longitude
				ret.lon = atof (tokenStart);
				break;
			case 4: // E/w Indicator
				ret.ew = (memcmp (tokenStart, "W", 1) == 0);
				break;
			case 5: // UTC Position
				break;
			case 6: // Fix Status
			    ret.iv = (memcmp (tokenStart, "V", 1) == 0);
				break;
			case 7: // Fix Mode
			    ret.fm = parseGY_GPSMV2_GLL_FixMode (tokenStart);
			    break;
			default:
				break;
			}
		}

		tokenStart = tokenEnd + 1;
		++i;
	}
}

GY_GPS6MV2_GLL_FixMode_t parseGY_GPSMV2_GLL_FixMode (const char *str) noexcept {
	if (strncmp (str, "A", 1) == 0) {
		return GLL_FIX_MODE_T_AUTONOMOUS;
	} else if (strncmp (str, "N", 1) == 0) {
		return GLL_FIX_MODE_T_NO_FIX;
	} else if (strncmp (str, "D", 1) == 0) {
		return GLL_FIX_MODE_T_DGPS;
	} else if (strncmp (str, "E", 1) == 0) {
		return GLL_FIX_MODE_T_DR;
	}

	return GLL_FIX_MODE_T_INVALID;
}

GY_GPS6MV2_UnitStep_t parseGY_GPS6MV2_UnitStep (const char *str) {
	if (strncmp (str, "MM", 2) == 0) {
		return UNIT_STEP_MM;
	} else if (strncmp (str, "CM", 2) == 0) {
		return UNIT_STEP_CM;
	} else if (strncmp (str, "M", 1) == 0) {
		return UNIT_STEP_M;
	} else if (strncmp (str, "KM", 2) == 0) {
		return UNIT_STEP_KM;
	}

	return UNIT_STEP_INVALID;
}

GY_GPS6MV2_Record_t parseGY_GPS6MV2_RecordType (const char *str) {
	if (strncmp (str, "GP", 2) == 0) {
		str = &str[2];
		if (strncmp (str, "GGA", 3) == 0) {
			return GGA;
		} else if (strncmp (str, "GLL", 3) == 0) {
			return GLL;
		} else if (strncmp (str, "GSA", 3) == 0) {
			return GSA;
		} else if (strncmp (str, "GSV", 3) == 0) {
			return GSV;
		} else if (strncmp (str, "RMC", 3) == 0) {
			return RMC;
		} else if (strncmp (str, "VTG", 3) == 0) {
			return VTG;
		} else if (strncmp (str, "ZDA", 3) == 0) {
			return ZDA;
		}
	}

	return INVALID;
}

/**************************************************
 * Classes
 **************************************************/

/// Creates a new GY GPS6MV2 Driver instance.
GY_GPS6MV2::GY_GPS6MV2 (USART_TypeDef *usart) noexcept:
	USART::USART (usart), m_LineBufferLevel (0)
{
	memset (&m_GGA, 0, sizeof (GY_GPS6MV2_GGA_Data_t));
}

/// Performs the driver initialization procedure.
void GY_GPS6MV2::Init (uint32_t clk) noexcept {
	// Calls the parent initialization procedure.
	USART::Init (clk, GY_GPS6MV2_BAUD);

	// Enables the RXNE interrupt.
	m_USART->CR1 |= USART_CR1_RXNEIE;
}

/// gets the GGA record data.
const GY_GPS6MV2_GGA_Data_t &GY_GPS6MV2::getGGA (void) const noexcept {
	return m_GGA;
}


/// Gets the GGL record data.
const GY_GPS6MV2_GLL_t &GY_GPS6MV2::getGLL (void) const noexcept {
	return m_GLL;
}

/// Gets called when a new char is received by the USART interrupt.
void GY_GPS6MV2::HandleISR (char c) noexcept {
	if (m_LineBufferLevel >= GY_GPS6MV2_BUFFER_SIZE) {
		m_LineBufferLevel = 0;
	}

	// Adds the char to the line buffer.
	m_LineBuffer[m_LineBufferLevel++] = c;

	// Checks if we've received a CRLF, if not: return from method.
	if (m_LineBufferLevel < 2) return; // [Make sure we have enough bytes].
	else if (memcmp (&m_LineBuffer[m_LineBufferLevel - 2], "\r\n", 2) != 0) return;	

	// Since we now know that we've received a CRLF, we and we want to perform further
	//  processing, we will change the '\r' into a null termination char.
	m_LineBuffer[m_LineBufferLevel - 2] = '\0';

	// Calls the handle record method, and clears the buffer after.
	GY_GPS6MV2::HandleRecord ();
	m_LineBufferLevel = 0;
}

/// Gets called once a new record is available to read.
void GY_GPS6MV2::HandleRecord (void) noexcept {
	//
	// Checksum Validation Procedure.
	//

	// Calculates and compares the checksum to ensure data integrity, if it is not
	//  valid discard the packet.
	{
		char *cPos;
		uint16_t cIndex;
		uint8_t originalCS, calcCS;

		// Gets the checksum position in the packet, if the position is not found
		//  we will set the line buffer level, and continue reading.
		
		cPos = strchr (m_LineBuffer, '*');
		if (cPos == NULL) {
			return;
		}

		cIndex = (cPos - m_LineBuffer) + 1;
		if (cIndex >= m_LineBufferLevel) {
			return;
		}

		originalCS = ParseChecksum (&m_LineBuffer[cIndex]) ;
		m_LineBuffer[cIndex - 1] = '\0'; // Remove CS from string.

		// Compares the original checksum against the one we calculated, if it does
		//  not match, return and set buffer level to zero.
		
		cPos = strchr (m_LineBuffer, '$');
		if (cPos == NULL) {
			return;
		}

		cIndex = (cPos - m_LineBuffer) + 1;
		if (cIndex >= m_LineBufferLevel) {
			return;
		}

		calcCS = GY_GPS6MV2::CalculateChecksum (&m_LineBuffer[cIndex]);
		if (originalCS != calcCS) {
			return;
		}
	}

	//
	// Data Parsing Procedure.
	//

	switch (parseGY_GPS6MV2_RecordType(&m_LineBuffer[1])) {
	case GGA:
		parseGY_GPSMV2_GGA_Record (m_LineBuffer, m_GGA);
		break;
	case GLL:
		parseGY_GPSMV2_GLL_Record (m_LineBuffer, m_GLL);
		break;
	default:
		break;
	}
}

/// Calculates the NMEA 0183 Checksum
uint8_t GY_GPS6MV2::CalculateChecksum (const char *s) {
	uint8_t cs = 0;

	while (*s) {
		cs ^= *s++;
	}

	return cs;
}

/// Parses the checksum from NMEA 0183 packet.
uint8_t GY_GPS6MV2::ParseChecksum (const char *cs) {
	return (Hex::FromChar (cs[0]) << 4) | Hex::FromChar(cs[1]);
}
