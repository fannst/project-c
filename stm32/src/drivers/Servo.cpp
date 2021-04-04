#include "drivers/Servo.hpp"

/// Creates a new servo cluster instance.
ServoCluster::ServoCluster (TIM_TypeDef *tim, float degrees, float min, float max) noexcept:
	m_TIM (tim), m_Degrees (degrees), m_Min (min), m_Max (max)
{}

/// Initializes the timer to serve the servos.
void ServoCluster::Init (uint16_t presc, uint16_t arr) noexcept {
  // Configures the prescalar, auto-reload register and the capture compare register.
  m_TIM->CNT = 0;               // Start counting at zero.
  m_TIM->PSC = presc;
  m_TIM->ARR = arr;
  m_TIM->CCR1 = m_TIM->CCR2 = m_TIM->CCR3 = m_TIM->CCR4 = (uint16_t) ((((float) arr) / 20.f) * m_Min);

  // OCx polarity is software programmable using the CCxP bit in the TIMx_CCER register. It
  // can be programmed as active high or active low. OCx output is enabled by the CCxE bit in
  // the TIMx_CCER register. Refer to the TIMx_CCERx register description for more details.

  // Enables the Capture/Compare thingy, with active-high.
  m_TIM->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;

  // The PWM mode can be selected independently on each channel (one PWM per OCx
  // output) by writing 110 (PWM mode 1) or ‘111 (PWM mode 2) in the OCxM bits in the
  // TIMx_CCMRx register. The corresponding preload register must be enabled by setting the
	// OCxPE bit in the TIMx_CCMRx register, and eventually the auto-reload preload register (in
	// upcounting or center-aligned modes) by setting the ARPE bit in the TIMx_CR1 register.

	// Sets the PWM mode to 1 (0b110), and enables the auto-reload preload register.
	m_TIM->CCMR1 |= ((0b110 << TIM_CCMR1_OC1M_Pos)
	| (0b110 << TIM_CCMR1_OC2M_Pos)
	| TIM_CCMR1_OC1PE
	| TIM_CCMR1_OC2PE);

	m_TIM->CCMR2 |= ((0b110 << TIM_CCMR2_OC3M_Pos)
	| (0b110 << TIM_CCMR2_OC4M_Pos)
	| TIM_CCMR2_OC3PE
	| TIM_CCMR2_OC4PE);

	// Sets the ARPE bit in the control register.
	m_TIM->CR1 |= TIM_CR1_ARPE;

	// As the preload registers are transferred to the shadow registers only when an update event
	// occurs, before starting the counter, all registers must be initialized by setting the UG bit in
	// the TIMx_EGR register.

	// Generate update event.
	m_TIM->EGR |= TIM_EGR_UG;

	// Enables the timer
	m_TIM->CR1 |= TIM_CR1_CEN;
}

/// Moves servo N to specified amount of degrees.
void ServoCluster::MoveServo (uint8_t n, float deg) noexcept {
  float v = m_Min + ((m_Max - m_Min) * (deg / m_Degrees));
  (&m_TIM->CCR1)[n] = (uint32_t) ((((float) m_TIM->ARR) / 20.f) * v);
}
