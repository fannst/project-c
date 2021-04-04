#include "hardware/SPI.hpp"

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

/// Creates a new SPI slave instance.
SPI_Slave::SPI_Slave (SPI_TypeDef *spi) noexcept:
	SPI (spi),
	m_FIFO (m_FIFOBuffer, SPI_SLAVE_FIFO_BUFFER_SIZE)
{}

/// Checks if there is a packet available in the FIFO.
bool SPI_Slave::PacketAvailable (void) const noexcept {

}

/// Reads a packet from the FIFO.
void SPI_Slave::ReadPacket (SPI_Slave_Packet_t *output) noexcept {

}