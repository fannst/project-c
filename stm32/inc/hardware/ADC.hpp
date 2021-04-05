/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#pragma once

class ADCn {
public:
	/// Creates ne ADC driver instance.
	ADCn (ADC_TypeDef *adc) noexcept;

	/// Initializes the ADC peripheral.
	void Init (void) noexcept;

	/// Enables the ADC.
	void Enable (void) noexcept;

	/// Disables the ADC.
	void Disable (void) noexcept;

	/// Starts the ADC conversion.
	void Start (void) noexcept;

	/// Stops the ADC conversion.
	void Stop (void) noexcept;
private:
	ADC_TypeDef *m_ADC;
};
