#include <stdnoreturn.h>
#include <stdbool.h>

#include "MKL05Z4.h"

#include "gpio.h"
#include "utility.h"
#include "rtc.h"

// Active wait time for each of the displays
#define DISPLAY_DELAY 100
// Maximum brightness
#define MAX_BRIGHTNESS 10

static void clock_tick(void);
static void bright_wait(void);
static void init(void);
noreturn static void endless_error(void);
noreturn static void endless_time(void);

// Hours high (first digit)
static unsigned hh = 0;
// Hours low (second digit)
static unsigned hl = 0;
// Minutes high (first digit)
static unsigned mh = 0;
// Minutes low (second digit)
static unsigned ml = 0;

static unsigned brightness = 10;

int main(void)
{
	init();

	endless_time();

    // Shouldn't get here. Show error. Never leave main.
	endless_error();
}

void RTC_IRQHandler(void) {
	if (RTC_SR & RTC_SR_TAF_MASK) {
		RTC_TAR += TICK_LEN;
		clock_tick();
	}
}

static void clock_tick(void) {
	++ml;
	if (ml < 10) {
		return;
	}

	ml = 0;
	++mh;
	if (mh < 6) {
		return;
	}

	mh = 0;
	++hl;
	if ((hl < 10 && hh < 2) || (hl < 4 && hh < 3)) {
		return;
	}

	hl = 0;
	++hh;
	if (hh < 3) {
		return;
	}

	hh = 0;
}

static void bright_wait(void) {
	active_wait(brightness * DISPLAY_DELAY);
	show(DIG_NONE, DIS_NONE);
	active_wait((MAX_BRIGHTNESS - brightness) * DISPLAY_DELAY);
}

static void init(void) {
	// Disable watchdog
	SIM_COPC = 0;

	gpio_init();
	rtc_init();
}

noreturn static void endless_time(void) {
    while (1) {
		show(DIGIT[hh], DIS_0);
		bright_wait();
		show(DIGIT[hl] | SEG_DP, DIS_1);
		bright_wait();
		show(DIGIT[mh], DIS_2);
		bright_wait();
		show(DIGIT[ml], DIS_3);
		bright_wait();
    }
}

noreturn static void endless_error(void) {
    while (true) {
		show(DIG_E, DIS_0);
		active_wait(DISPLAY_DELAY);
		show(DIG_r, DIS_1);
		active_wait(DISPLAY_DELAY);
		show(DIG_o, DIS_2);
		active_wait(DISPLAY_DELAY);
		show(DIG_r, DIS_3);
		active_wait(DISPLAY_DELAY);
    }
}
