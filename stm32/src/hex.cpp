#include "hex.hpp"

/// Returns the 4-bit value from a HEX char.
uint8_t Hex::FromChar (char c) noexcept {
	if (c >= '0' && c <= '9') {
		return c - '0';
	} else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	} else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	} else {
		return 0;
	}
}