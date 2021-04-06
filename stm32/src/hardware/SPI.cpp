#include "hardware/SPI.hpp"

/**************************************************
 * Global Variables
 **************************************************/

SPI_Slave SPI_Slave::INSTANCE (SPI1);

/**************************************************
 * Interrupt Handlers
 **************************************************/

extern "C" void SPI1_IRQHandler (void) noexcept {
	if (SPI1->SR & SPI_SR_RXNE) { // RXNE Flag set (byte available)
		uint8_t byte = *reinterpret_cast<volatile uint8_t *> (&SPI1->DR);
		SPI_Slave::GetInstance ().HandleIRQ (byte);
	}
}

/// Handles the interrupt request of DMA1 Stream0.
extern "C" void DMA1_Stream0_IRQHandler (void) noexcept {
	if (DMA1->LISR & DMA_LISR_TCIF0) { // Transmit Complete Interrupt for Stream 0
		SPI_Slave::GetInstance ().HandleDMAIRQ ();
		DMA1->LIFCR |= DMA_LIFCR_CTCIF0;
	}
}

/**************************************************
 * SPI Master Class Methods
 **************************************************/

/// Creates a new SPI instance.
SPI::SPI (SPI_TypeDef *spi) noexcept:
	m_SPI (spi)
{}

/// Initializes the SPI peripheral.
void SPI::Init (void) noexcept {
	m_SPI->CR2 = ((0b0111 << SPI_CR2_DS_Pos));		// 8-bit data size
	m_SPI->CR1 = (SPI_CR1_SPE);						// Enable SPI
}

/// Writes a single byte of data to the SPI bus.
void SPI::Write (uint8_t byte) noexcept {
	*reinterpret_cast<volatile uint8_t *> (&m_SPI->DR) = byte;
	while (!(m_SPI->SR & SPI_SR_TXE));
}

/// Reads a single byte of the SPI bus.
uint8_t SPI::Read (void) noexcept {
	while (!(m_SPI->SR & SPI_SR_RXNE));
	return *reinterpret_cast<volatile uint8_t *> (&m_SPI->DR);
}

/**************************************************
 * SPI Slave Methods
 **************************************************/

/// Creates a new SPI slave instance.
SPI_Slave::SPI_Slave (SPI_TypeDef *spi) noexcept:
	SPI (spi),
	m_FIFO (m_FIFOBuffer, SPI_SLAVE_FIFO_BUFFER_SIZE),
	m_PacketBufferLevel (0)
{}

/// Initializes the SPI slave.
void SPI_Slave::Init (void) noexcept {

}

/// Checks if there is a packet available in the FIFO.
bool SPI_Slave::PacketAvailable (void) const noexcept {

}

/// Reads a packet from the FIFO.
void SPI_Slave::ReadPacket (SPI_Slave_Packet_t *output) noexcept {

}

/// Gets called on a RXNE interrupt.
void SPI_Slave::HandleIRQ (uint8_t byte) noexcept {
	// Adds the current byte to the final packet buffer.
	m_PacketBuffer[m_PacketBufferLevel++] = byte;

	// Checks if we've received the length of the currently
	//  incomming packet, if so activate the DMA.
	if (m_PacketBufferLevel == 2) {
		m_PacketLength = (static_cast<uint16_t>(m_PacketBuffer[0]) << 8)
			| static_cast<uint16_t>(m_PacketBuffer[1]);

		// Configures the DMA for a N bytes transfer into our packet buffer.
		DMA1_Stream0->M0AR = reinterpret_cast<uintptr_t>(m_PacketBuffer);
		DMA1_Stream0->NDTR = m_PacketLength;
	}
}

/// Gets called on a DMA interrupt.
void SPI_Slave::HandleDMAIRQ (void) noexcept {
	// Writes the currently received bytes to the FIFO, and resets
	//  the level of the packet buffer.

	m_FIFO.WriteBytes (m_PacketBuffer, m_PacketLength);
	m_PacketBufferLevel = 0;

	// Re-Enables the SPI1 RXNE interrupt.
	SPI1->CR2 |= SPI_CR2_RXNEIE;
}
