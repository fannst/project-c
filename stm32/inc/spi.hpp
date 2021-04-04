/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#pragma once

class SPI_Slave {
public:
	void Init (void) noexcept;

	void Write (uint8_t b) const noexcept;
	uint8_t Read (void) const noexcept;
private:
	SPI_TypeDef *m_SPI;
};
