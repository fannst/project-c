/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"
#include "sysclk.hpp"

#pragma once

/**************************************************
 * Global USART configuration
 **************************************************/

#define __USART_STDOUT_BUSFREQ		(__SYSCLOCK_APB2_PERIPHERALS * 1000000)
#define __USART_STDOUT_BAUD			(921600)

/**************************************************
 * Class Definitions
 **************************************************/

class USART {
public:

	/// Creates a new USART instance.
	USART (USART_TypeDef *usart) noexcept;

	// Initializes the USART peripheral with the specified baud rate.
	virtual void Init (uint32_t clk, uint32_t baud);

	/// Writes an single char to the USART peripheral.
	virtual void Write (uint8_t c) const;

	/// Writes N bytes to the USART peripheral.
	virtual void Write (const uint8_t *s, uint16_t n) const;

	/// Writes an string to the USART peripheral.
	virtual void Write (const char *s) const;

	/// Reads an byte from the USART peripheral.
	virtual uint8_t Read (void) const;

	/// Initializes the STDOUT USART (1).
	static void STDUsartInit (void) noexcept;

public:
	static USART STDOUT;
protected:
	USART_TypeDef *m_USART;
};

class USART_DMA : public USART {
public:

	/// Creates a new USART_DMA instance.
	USART_DMA (USART_TypeDef *usart, DMA_Stream_TypeDef *stream, uint8_t *txBuffer) noexcept;

	/// Initializes the USART hardware and the DMA stream.
	virtual void Init (uint32_t clk, uint32_t baud, uint8_t channel);

	/// Writes N bytes to the USART peripheral.
	virtual void Write (const uint8_t *s, uint16_t n) const;

	/// Writes an string to the USART peripheral.
	virtual void Write (const char *s) const;

protected:
	DMA_Stream_TypeDef *m_DMAStream;
	uint8_t *m_TXBuffer;
};
