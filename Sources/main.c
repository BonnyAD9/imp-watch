#include <stdnoreturn.h>
#include <stdbool.h>
#include <stddef.h>

#include "MKL05Z4.h"

#include "gpio.h"
#include "utility.h"
#include "rtc.h"

// Active wait time for each of the displays
#define DISPLAY_DELAY 100
// Maximum brightness
#define MAX_BRIGHTNESS 10
// How long to wait for button value to stabilize
#define PRESS_DELAY 100

typedef enum {
	STATE_PRIMARY = 0xff00,
	STATE_SECONDARY = 0xff,
	STATE_SHOW_TIME = 0x0,
	STATE_EDIT_TIME = 0x100,
	STATE_EDIT_TIME_HH = 0x100,
	STATE_EDIT_TIME_HL,
	STATE_EDIT_TIME_MH,
	STATE_EDIT_TIME_ML,
} State;

static void btn_change(void);
static void display(void);
static void short_press(void);
static void long_press(void);
static void clock_tick(void);
static void clock_truncate(void);
static void bright_wait(void);
static void init(void);
static void show_err(void);
static void show_time(size_t dot);

static State state = STATE_SHOW_TIME;

static unsigned time[] = { 1, 5, 5, 7 };

// Hours high (first digit)
#define HH (time[0])
// Hours low (second digit)
#define HL (time[1])
// Minutes high (first digit)
#define MH (time[2])
// Minutes low (second digit)
#define ML (time[3])

static unsigned brightness = 10;

int main(void)
{
	init();

	while (true) {
		display();
	}
}

void RTC_IRQHandler(void) {
	if (RTC_SR & RTC_SR_TAF_MASK) {
		RTC_TAR += TICK_LEN;
		clock_tick();
	}
}

void PORTB_IRQHandler(void) {
	active_wait(PRESS_DELAY);

	btn_change();

	PORTB_ISFR = PORTB_ISFR;
}

static void btn_change(void) {
	if (!(PORTB_ISFR & SEG_BTN) || !IS_PRESSED) {
		return;
	}

	uint32_t press_time = NOW;
	while (IS_PRESSED) {
		display();
		if (NOW - press_time >= 2) {
			long_press();
			return;
		}
	}
	short_press();
}

static void display(void) {
	switch (state & STATE_PRIMARY) {
		case STATE_SHOW_TIME:
			show_time(1);
			break;
		case STATE_EDIT_TIME:
			show_time(state & STATE_SECONDARY);
			break;
		default:
			show_err();
			break;
	}
}

static void short_press(void) {
	switch (state & STATE_PRIMARY) {
		case STATE_SHOW_TIME:
			break;
		case STATE_EDIT_TIME:
			++time[state & STATE_SECONDARY];
			clock_truncate();
			break;
	}
}

static void long_press(void) {
	switch (state) {
		case STATE_SHOW_TIME:
			rtc_tick(false);
			state = STATE_EDIT_TIME;
			break;
		case STATE_EDIT_TIME_HH:
		case STATE_EDIT_TIME_HL:
		case STATE_EDIT_TIME_MH:
			++state;
			break;
		case STATE_EDIT_TIME_ML:
			state = STATE_SHOW_TIME;
			RTC_TAR = NOW + TICK_LEN;
			rtc_tick(true);
			break;
	}
}

static void clock_tick(void) {
	++ML;
	if (ML < 10) {
		return;
	}

	ML = 0;
	++MH;
	if (MH < 6) {
		return;
	}

	MH = 0;
	++HL;
	if ((HL < 10 && HH < 2) || (HL < 4 && HH < 3)) {
		return;
	}

	HL = 0;
	++HH;
	if (HH < 3) {
		return;
	}

	HH = 0;
}

static void clock_truncate(void) {
	if (ML >= 10) {
		ML = 0;
	}

	if (MH >= 10) {
		MH = 0;
	}

	if (HH > 2) {
		HH = 0;
	}

	if (HL >= 10 || (HH >= 2 && HL >= 4)) {
		HL = 0;
	}
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
		show_time(1);
    }
}

noreturn static void endless_error(void) {
    while (true) {
		show_err();
    }
}

static void show_time(size_t dot) {
	for (size_t i = 0; i < DISPLAY_LEN; ++i) {
		Digit base = i == dot ? SEG_DP : DIG_NONE;
		show(DIGIT[time[i]] | base, DISPLAY[i]);
		bright_wait();
	}
}

static void show_err(void) {
	show(DIG_E, DIS_0);
	bright_wait();
	show(DIG_r, DIS_1);
	bright_wait();
	show(DIG_o, DIS_2);
	bright_wait();
	show(DIG_r, DIS_3);
	bright_wait();
}
