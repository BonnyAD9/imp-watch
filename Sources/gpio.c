#include "gpio.h"

#include "MKL05Z4.h"

void gpio_init(void) {
	// Enable clock on ports
	SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;

	// Port configuration for ports. Enable on GPIO.
	enum { PCR = PORT_PCR_MUX(1) };

	PORTA_PCR8 = PCR;
	PORTA_PCR9 = PCR;
	PORTA_PCR10 = PCR;
	PORTA_PCR11 = PCR;

	PORTB_PCR0 = PCR;
	PORTB_PCR1 = PCR;
	PORTB_PCR2 = PCR;
	PORTB_PCR3 = PCR;
	PORTB_PCR8 = PCR;
	PORTB_PCR9 = PCR;
	PORTB_PCR10 = PCR;
	PORTB_PCR11 = PCR;

	// Enable ports as output.
	GPIOA_PDDR = DIS_ALL;
	GPIOB_PDDR = DIG_ALL;
}
