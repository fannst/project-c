#include "Dog.hpp"

Dog::Dog (void) noexcept:
	m_PawFrontLeft (TIM2, 180.F, .5F, 2.4F),
	m_PawFrontRight (TIM3, 180.F, .5F, 2.4F),
	m_PawBackLeft (TIM4, 180.F, .5F, 2.4F),
	m_PawBackRight (TIM5, 180.F, .5F, 2.4F)
{}

void Dog::Init (void) noexcept {
	//
	// Initializes the paws.
	//

	// APB1 (After Clock Config) = 108Mhz (For Timers)
	// | 108Mhz / 1800 = 60Khz
	// | 60Khz / 50 = 1.2Khz (Each 20ms)
	
	#define DOG_PAW_TIMER_PRESC			(1800)
	#define DOG_PAW_TIMER_ARR			(1200)

	m_PawFrontLeft.Init (DOG_PAW_TIMER_PRESC, DOG_PAW_TIMER_ARR);
	m_PawFrontRight.Init (DOG_PAW_TIMER_PRESC, DOG_PAW_TIMER_ARR);
	m_PawBackLeft.Init (DOG_PAW_TIMER_PRESC, DOG_PAW_TIMER_ARR);
	m_PawBackRight.Init (DOG_PAW_TIMER_PRESC, DOG_PAW_TIMER_ARR);

	//
	// Initializes the distance sensors.
	//

	//
	// Initializes the paw pressure sensors.
	//	
}