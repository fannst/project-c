/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#include "datatypes/FIFO.hpp"

#pragma once

typedef enum {
	SPI_BR_DIV2 	= 0b000,
	SPI_BR_DIV4 	= 0b001,
	SPI_BR_DIV8 	= 0b010,
	SPI_BR_DIV16 	= 0b011,
	SPI_BR_DIV32 	= 0b100,
	SPI_BR_DIV64 	= 0b101,
	SPI_BR_DIV128 	= 0b110,
	SPI_BR_DIV256 	= 0b111
} SPI_BR_t;

class SPI {
public:

	/// Creates a new SPI instance.
	SPI (SPI_TypeDef *spi) noexcept;

	/// Initializes the SPI peripheral.
	virtual void Init (void) noexcept;

	/// Writes a single byte of data to the SPI bus.
	void Write (uint8_t byte) noexcept;

	/// Reads a single byte of the SPI bus.
	uint8_t Read (void) noexcept;

protected:
	SPI_TypeDef *m_SPI;
};

typedef enum {
	SPI_SLAVE_PACKET_SERVICE_MANAGEMENT = 0, 	/* Management Stuff */
	SPI_SLAVE_PACKET_SERVICE_GPS = 1,			/* GPS Stuff */
	SPI_SLAVE_PACKET_SERVICE_TOF = 2,			/* TOF (Time Of Flight) distance sensors */
	SPI_SLAVE_PACKET_SERVICE_SERVO = 3			/* Servo Control */
} SPI_Slave_Packet_Service_t;

typedef struct __attribute__ (( packed )) {
	uint16_t	tl;				/* The Total Packet Len */
	uint16_t	cs;				/* The Packet Checksum */
	uint8_t		service;		/* The Service the packet is meant for */
	uint8_t 	payload[0];		/* The Packet Payload */
} SPI_Slave_Packet_t;

typedef struct __attribute__ (( packed )) {

} SPI_Slave_Management_Payload_t;

#define SPI_SLAVE_FIFO_BUFFER_SIZE				(1024)
#define SPI_SLAVE_PACKET_BUFFER_SIZE			(98)

class SPI_Slave : public SPI {
public:
	inline static SPI_Slave &GetInstance (void) noexcept {
		return INSTANCE;
	}

	/// Checks if there is a packet available in the FIFO.
	bool PacketAvailable (void) const noexcept;

	/// Reads a packet from the FIFO.
	void ReadPacket (SPI_Slave_Packet_t *output) noexcept;

	/// Initializes the SPI slave.
	virtual void Init (void) noexcept;

	/// Gets called on a RXNE interrupt.
	void HandleIRQ (uint8_t byte) noexcept;

	/// Gets called on a DMA interrupt.
	void HandleDMAIRQ (void) noexcept;
protected:
	/// Creates a new SPI slave instance.
	SPI_Slave (SPI_TypeDef *spi) noexcept;

	static SPI_Slave INSTANCE;

	uint8_t m_PacketBuffer[SPI_SLAVE_PACKET_BUFFER_SIZE];
	uint8_t m_FIFOBuffer[SPI_SLAVE_FIFO_BUFFER_SIZE];
	uint32_t m_PacketBufferLevel;
	uint16_t m_PacketLength;
	FIFO m_FIFO;
};