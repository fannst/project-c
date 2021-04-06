#include "main.hpp"

#include "inc.hpp"

/**************************************************
 * Static Variables
 **************************************************/

Main Main::INSTANCE;

/**************************************************
 * Interrupt Service Routines
 **************************************************/

/// Handles the USART2 Interrupt.
extern "C" void USART2_IRQHandler (void) noexcept {
	if (USART2->ISR & USART_ISR_RXNE) {
		char c = *reinterpret_cast<volatile char *> (&USART2->RDR);
		
		Main::GetInstance().getGPS().HandleISR (c);
	}
}

/**************************************************
 * Classes
 **************************************************/

/// Gets the curernt singleton instance.
Main &Main::GetInstance() noexcept {
	return Main::INSTANCE;
}

/// Gets the GPS instance.
GY_GPS6MV2 &Main::getGPS() noexcept {
	return m_GPS;
}

Main::Main (void) noexcept:
	m_I2C (I2C1),
	m_IMU (m_I2C, BNO055_DEFAULT_I2C_ID),
	m_GPS (USART2),
	m_Buzzer (GPIOA, 1)
{}

/// Initializes the GPS hardware.
void Main::GPSInit (void) noexcept {
	//
	// Initializes the GPIO.
	//
	
	// Makes PA2 (TX), PA3 (RX) Alternative Function
	GPIOA->MODER &= ~(GPIO_MODER_MODER2_Msk 
		| GPIO_MODER_MODER3_Msk);
	GPIOA->MODER |= ((0x2 << GPIO_MODER_MODER2_Pos)
		| (0x2 << GPIO_MODER_MODER3_Pos));

	// Selects AF7 for both A2 (TX) and A3 (RX).
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2_Msk
		| GPIO_AFRL_AFRL3_Msk);
	GPIOA->AFR[0] |= ((7 << GPIO_AFRL_AFRL2_Pos)
		| (7 << GPIO_AFRL_AFRL3_Pos));

	//
	// Initializes the driver.
	//
	
	// Enables the IRQ in the NVIC.
	NVIC_EnableIRQ (USART2_IRQn);

	// Initializes the GPS driver.
	m_GPS.Init ((__SYSCLOCK_APB1_PERIPHERALS * 1000000));
}

/// Initializes the SPI slave peripheral.
void Main::SPISlaveInit (void) noexcept {
	//
	// Initializes the GPIO.
	//
	
	// Makes PA(4/5/6/7) Alternative Function mode.
	GPIOA->MODER &= ~(GPIO_MODER_MODER4_Msk 
		| GPIO_MODER_MODER5_Msk
		| GPIO_MODER_MODER6_Msk
		| GPIO_MODER_MODER7_Msk);
	GPIOA->MODER |= ((0x2 << GPIO_MODER_MODER4_Pos)
		| (0x2 << GPIO_MODER_MODER5_Pos)
		| (0x2 << GPIO_MODER_MODER6_Pos)
		| (0x2 << GPIO_MODER_MODER7_Pos));

	// Selects AF5 for PA(4/5/6/7) to select SPI1
	GPIOA->AFR[0] = ((5 << GPIO_AFRL_AFRL4_Pos)
		| (5 << GPIO_AFRL_AFRL5_Pos)
		| (5 << GPIO_AFRL_AFRL6_Pos)
		| (5 << GPIO_AFRL_AFRL7_Pos));

	//
	// Initializes the peripheral.
	//

	// Enables the SPI1 interrupt.
	NVIC_EnableIRQ (SPI1_IRQn);

	// Initializes the SPI slave.
}

/// Initializes the I2C Master Peripheral
void Main::I2CMasterInit (void) noexcept {
	//
	// Initializes the GPIO.
	//

	// Makes PB6 (SCL), PB7 (SDA) Alternative Function
	GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
	GPIOB->MODER |= ((0x2 << GPIO_MODER_MODER6_Pos)
		| (0x2 << GPIO_MODER_MODER7_Pos));

	// Selects AF4 for PB6 and PB7.
	GPIOB->AFR[0] |= ((4 << GPIO_AFRL_AFRL6_Pos)
		| (4 << GPIO_AFRL_AFRL7_Pos));

	//
	// Initializes the peripheral
	//

	// Initializes the I2C peripheral.
	m_I2C.Init ();
}

/// Enables the used peripheral clocks.
void Main::SetupRCC (void) noexcept {
	// Enables GPIOA, DMA1
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN
		| RCC_AHB1ENR_GPIOBEN
		| RCC_AHB1ENR_DMA1EN);

	// Enables TIM8 (Delay)
	RCC->APB2ENR |= (RCC_APB2ENR_TIM8EN);

	// Enables USART1 (STDOUT)
	RCC->APB2ENR |= (RCC_APB2ENR_USART1EN);

	// Enables WWDG (Watchdog), USART2 (GPS), I2C1 (TOF 1)
	RCC->APB1ENR |= (RCC_APB1ENR_WWDGEN
		| RCC_APB1ENR_USART2EN
		| RCC_APB1ENR_I2C1EN);
}

/// Handles the setup code.
void Main::Setup (void) noexcept {
	// Shepherd::Init ();
	Delay::Init ();

	USART::STDUsartInit ();
	Main::SPISlaveInit ();
	Main::GPSInit ();
	Main::I2CMasterInit ();

	m_Buzzer.Init ();
	m_IMU.Init ();

	// Performs the startup beep, and prints that we're ready
	//  to the serial terminal.
	m_Buzzer.Buzz (250, 500);
	printf("STM32 CTRL Ready!\r\n");
}

/// Gets called forever.
void Main::Loop (void) noexcept {
	Shepherd::Kick ();

	const GY_GPS6MV2_GGA_Data_t &gga = m_GPS.getGGA ();
	printf ("Lon: %f, Lat: %f, nSat: %u\r\n", gga.lon, gga.lat, gga.nSatelites);

	Delay::Ms (500);
}

/**************************************************
 * Default Functions
 **************************************************/

/// Gets called after the startup code is finished.
extern "C" int main (void) noexcept {
	// Initializes the system clock to 216Mhz using the PLL.
	SysClk::Init ();

	// Gets the singleton instance from main.
	Main &main = Main::GetInstance ();

	// Initializes the RCC (Peripheral Clock Supplies) and
	//  calls the initial setup method.
	main.SetupRCC ();
	main.Setup ();

	// Stays in loop forever, and calls the loop of main.
	while (true) {
		main.Loop ();
	}

	// Should never be reached LOL.
	return 0;
}