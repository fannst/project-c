#include "hardware/ADC.hpp"

/// Creates ne ADC driver instance.
ADCn::ADCn (ADC_TypeDef *adc) noexcept:
	m_ADC (adc)
{}

/// Initializes the ADC peripheral.
void ADCn::Init (void) noexcept {
	m_ADC->CR1 = ();
}

/// Enables the ADC.
void ADCn::Enable (void) noexcept {
	m_ADC->CR2 |= ADC_CR2_ADON;
}

/// Disables the ADC.
void ADCn::Disable (void) noexcept {
	m_ADC->CR2 &= ~ADC_CR2_ADON;
}

/// Starts the ADC conversion.
void ADCn::Start (void) noexcept {
	m_ADC->CR2 |= ADC_CR2_SWSTART;
}

/// Stops the ADC conversion.
void ADCn::Stop (void) noexcept {

}
