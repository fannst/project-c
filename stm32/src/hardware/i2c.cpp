#include "hardware/i2c.hpp"

I2C::I2C (I2C_TypeDef *i2c) noexcept:
	m_I2C (i2c)
{}

/// Initializes the I2C Peripheral.
void I2C::Init (void) noexcept {
	m_I2C->TIMINGR = ((14 << I2C_TIMINGR_PRESC_Pos)			// I2C Clock / 14
		| (14 << I2C_TIMINGR_SCLDEL_Pos)
		| (14 << I2C_TIMINGR_SDADEL_Pos)
		| (128 << I2C_TIMINGR_SCLH_Pos)
		| (128 << I2C_TIMINGR_SCLL_Pos)
	);
	m_I2C->CR1 = (I2C_CR1_PE);
}

/// Sets the target slave address,
void I2C::SetSlaveAddress (uint8_t sAddr) noexcept {
	m_I2C->CR2 &= ~(0x7F << (I2C_CR2_SADD_Pos));
	m_I2C->CR2 |= ((sAddr & 0x7F) << (I2C_CR2_SADD_Pos));
}

/// Generates an start condition.
void I2C::Start (void) noexcept {
	m_I2C->CR2 |= I2C_CR2_START;			// Triggers START send.
	while (m_I2C->CR2 & I2C_CR2_START);		// Waits for START send to finish.
}

/// Generates an stop condition.
void I2C::Stop (void) noexcept {
	m_I2C->CR2 |= I2C_CR2_STOP;				// Triggers STOP send.
	while (m_I2C->CR2 & I2C_CR2_STOP);		// Waits for STOP to finish.
}

/// Writes the specified byte to the I2C bus.
void I2C::Write (uint8_t byte) noexcept{
	*reinterpret_cast<volatile uint8_t *> (&m_I2C->TXDR) = byte;
	while (!(m_I2C->ISR & I2C_ISR_TXE));
}

/// Reads one byte from the I2C bus.
uint8_t I2C::Read (void) noexcept {
	while (!(m_I2C->ISR & I2C_ISR_RXNE));
	return *reinterpret_cast<volatile uint8_t *> (&m_I2C->RXDR);
}

/// Sets the I2C Mode.
void I2C::SetMode (I2C_Operation_Mode_t mode) noexcept {
	if (mode == I2C_OPMODE_WRITE) {
		m_I2C->CR2 &= ~I2C_CR2_RD_WRN;
	} else if (mode == I2C_OPMODE_READ) {
		m_I2C->CR2 |= I2C_CR2_RD_WRN;
	}
}