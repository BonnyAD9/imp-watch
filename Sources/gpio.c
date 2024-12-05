#include "gpio.h"

#include "MKL05Z4.h"

unsigned brightness = 10;

void gpio_init(void) {
	// Enable clock on ports
	SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;

	// Port configuration for ports. Enable on GPIO.
	enum { MUX_GPIO = 0x1, PCR = PORT_PCR_MUX(MUX_GPIO) };

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

	// Setup button input
	NVIC_DisableIRQ(PORTB_IRQn);
	enum { INT_ANY_EDGE = 0xb };
	PORTB_PCR4 = PORT_PCR_ISF(1)
		| PORT_PCR_IRQC(INT_ANY_EDGE)
		| PORT_PCR_MUX(MUX_GPIO)
		| PORT_PCR_PE_MASK
		| PORT_PCR_PS_MASK; // Pull up
	NVIC_ClearPendingIRQ(PORTB_IRQn);
	NVIC_EnableIRQ(PORTB_IRQn);
}
