#include "hardware/usart.hpp"

/**************************************************
 * Global Variables
 **************************************************/

USART USART::STDOUT (USART1);

/**************************************************
 * USART Master Class
 **************************************************/

/// Creates a new USART instance.
USART::USART (USART_TypeDef *usart) noexcept:
	m_USART (usart)
{}

// Initializes the USART peripheral with the specified baud rate.
void USART::Init (uint32_t clk, uint32_t baud) {
	// Calculates and sets the baud rate.
	m_USART->BRR = (((2 * clk) + baud) / (2 * baud));

	// Configures the USART peripheral further.
	m_USART->CR1 = USART_CR1_TE		// Transmit Enable
		| USART_CR1_RE				// Receive Enable
		| USART_CR1_UE;				// USART Enable (EN)
}

/// Writes an single char to the USART peripheral.
void USART::Write (uint8_t c) const {
	*reinterpret_cast<volatile uint8_t *>(&m_USART->TDR) = c;
	while (!(m_USART->ISR & USART_ISR_TXE));
}

/// Writes N bytes to the USART peripheral.
void USART::Write (const uint8_t *s, uint16_t n) const {
	for (uint16_t i = 0; i < n; ++i) {
		USART::Write(s[i]);
	}
}

/// Writes an string to the USART peripheral.
void USART::Write (const char *s) const {
	while (*s != '\0') {
		USART::Write(*s++);
	}
}

/// Reads an byte from the USART peripheral.
uint8_t USART::Read (void) const {
	while (!(m_USART->ISR & USART_ISR_RXNE));
	return *reinterpret_cast<volatile uint8_t *>(&m_USART->RDR);
}

/// Initializes the STDOUT USART (1).
void USART::STDUsartInit (void) noexcept {
	// Makes A9 (TX) and A10 (RX) Alternative Function
	GPIOA->MODER &= ~(GPIO_MODER_MODER9_Msk 
		| GPIO_MODER_MODER10_Msk);
	GPIOA->MODER |= ((0x2 << GPIO_MODER_MODER9_Pos)
		| (0x2 << GPIO_MODER_MODER10_Pos));

	// Selects AF7 for both A9 (TX) and A10 (RX).
	GPIOA->AFR[1] &= ~(GPIO_AFRH_AFRH1_Msk
		| GPIO_AFRH_AFRH2_Msk);
	GPIOA->AFR[1] |= ((7 << GPIO_AFRH_AFRH1_Pos)
		| (7 << GPIO_AFRH_AFRH2_Pos));

	// Performs the actual peripheral initialization.
	USART::STDOUT.Init (__USART_STDOUT_BUSFREQ, __USART_STDOUT_BAUD);
}

/**************************************************
 * USART w/ DMA
 **************************************************/

/// Creates a new USART_DMA instance.
USART_DMA::USART_DMA (
	USART_TypeDef *usart, DMA_Stream_TypeDef *stream, uint8_t *txBuffer
) noexcept:
	USART (usart), m_TXBuffer(txBuffer)
{}

/// Initializes the USART hardware and the DMA stream.
void USART_DMA::Init (uint32_t clk, uint32_t baud, uint8_t channel) {
	// Initializes the DMA stream.
	m_DMAStream->PAR = reinterpret_cast<uintptr_t>(&m_USART->TDR);		// Peripheral Address
	m_DMAStream->M0AR = reinterpret_cast<uintptr_t>(m_TXBuffer);		// TX Buffer Address
	m_DMAStream->CR = (DMA_SxCR_TCIE									// Transfer Complete Interrupt Enable
		| (0x1 << DMA_SxCR_DIR_Pos)										// Memory to Peripheral
		| DMA_SxCR_CIRC													// Circular Mode
		| DMA_SxCR_MINC													// Memory Increment Mode
		| (0x0 << DMA_SxCR_PSIZE_Pos)									// 8-bit Peripheral Memory
		| (0x0 << DMA_SxCR_MSIZE_Pos)									// 8-bit Memory
		| (0x1 << DMA_SxCR_PL_Pos)										// Medium Priority
		| (channel << DMA_SxCR_CHSEL_Pos)								// Select Channel
	);

	// Calls the parent to actually initialize the USART periperhal.
	USART::Init (clk, baud);

	m_USART->CR3 = USART_CR3_DMAT;
}

/// Writes N bytes to the USART peripheral.
void USART_DMA::Write (const uint8_t *s, uint16_t n) const {
	// Waits for the current transmission to end.
	while (m_DMAStream->CR & DMA_SxCR_EN);

	// Starts the new transmission.
	m_USART->ICR |= USART_ICR_TCCF;										// Clear the TC Flag
	m_DMAStream->NDTR = n;												// N bytes to transfer
	m_DMAStream->CR |= (DMA_SxCR_EN);									// Start Transfer
}

/// Writes an string to the USART peripheral.
void USART_DMA::Write (const char *s) const {
	USART_DMA::Write (reinterpret_cast<const uint8_t *>(s), strlen (s));
}

/**************************************************
 * IO Overrides
 **************************************************/

/// Overrides the IO putchar method (stdout).
extern "C" int __io_putchar (int ch) {
	USART::STDOUT.Write (static_cast<uint8_t> (ch));
	return 0;
}

/// Overrides the IO getchar method (stdout).
extern "C" int __io_getchar (void) {
	return static_cast<int> (USART::STDOUT.Read ());
}
