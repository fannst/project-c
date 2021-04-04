#include "inc.hpp"

#include "sysclk.hpp"
#include "buzzer.hpp"
#include "delay.hpp"

#include "hardware/wwdg.hpp"
#include "hardware/usart.hpp"

#include "drivers/GY-GPS6MV2.hpp"

static Buzzer buzzer (GPIOA, 1);

static GY_GPS6MV2 gps (USART2);

/**************************************************
 * Interrupt Service Routines
 **************************************************/

/// Handles the USART2 Interrupt.
extern "C" void USART2_IRQHandler (void) noexcept {
	if (USART2->ISR & USART_ISR_RXNE) {
		char c = *reinterpret_cast<volatile char *> (&USART2->RDR);
		
		gps.HandleISR (c);
	}
}

/**************************************************
 * Initialization Functions
 **************************************************/

/// Initializes the GY-GPS6MV Peripheral.
void __gy_gps6mv2_init (void) noexcept {
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
	gps.Init ((__SYSCLOCK_APB1_PERIPHERALS * 1000000));
}

/**************************************************
 * Default Functions
 **************************************************/

/// Enables the used peripheral clocks.
void __setup_rcc (void) noexcept {
	// Enables GPIOA, DMA1
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN
		| RCC_AHB1ENR_DMA1EN);

	// Enables TIM8 (Delay)
	RCC->APB2ENR |= (RCC_APB2ENR_TIM8EN);

	// Enables USART1 (STDOUT)
	RCC->APB2ENR |= (RCC_APB2ENR_USART1EN);

	// Enables WWDG (Watchdog), USART2 (GPS)
	RCC->APB1ENR |= (RCC_APB1ENR_WWDGEN
		| RCC_APB1ENR_USART2EN);
}

/// Handles the setup code.
void __setup (void) noexcept {
	// Shepherd::Init ();
	Delay::Init ();
	USART::STDUsartInit ();

	buzzer.Init ();

	__gy_gps6mv2_init ();

	// Performs the startup beep, and prints that we're ready
	//  to the serial terminal.
	buzzer.Buzz (250, 500);
	printf("STM32 CTRL Ready!\r\n");
}

/// Gets called forever.
void __loop (void) noexcept {
	Shepherd::Kick ();

	const GY_GPS6MV2_GGA_Data_t &gga = gps.getGGA ();
	printf ("Lon: %f, Lat: %f, nSat: %u\r\n", gga.lon, gga.lat, gga.nSatelites);

	Delay::Ms (500);
}

void __fpu_enable (void) noexcept {
	*((volatile uint32_t *) (0xE000ED88)) |= (0xF << 20);
}

/// Gets called after the startup code is finished.
extern "C" __attribute__ (( noreturn )) int main (void) noexcept {
	__fpu_enable ();

	SysClk::Init ();

	__setup_rcc ();
	__setup ();

	while (true) {
		__loop ();
	}
}