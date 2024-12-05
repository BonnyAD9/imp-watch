#include <stdnoreturn.h>
#include <stdbool.h>
#include <stddef.h>

#include "MKL05Z4.h"

#include "gpio.h"
#include "utility.h"
#include "rtc.h"
#include "clock.h"

/// @brief How long to wait for button value to stabilize
#define PRESS_DELAY 100

/// Run state
typedef enum {
	// State masks
	/// @brief Primary state mask
	STATE_PRIMARY = 0xff00,
	/// @brief Secondary state mask.
	STATE_SEC = 0xff,
	/// @brief Show time mode.
	STATE_SHOW_TIME = 0x0,
	/// @brief Edit time mode.
	STATE_EDIT_TIME = 0x100,
	STATE_EDIT_TIME_HH = 0x100,
	STATE_EDIT_TIME_HL,
	STATE_EDIT_TIME_MH,
	STATE_EDIT_TIME_ML,
	/// @brief Menu mode
	STATE_MENU = 0x200,
	STATE_MENU_TIME = 0x200,
	STATE_MENU_BRIGHTNESS = 0x201,
	/// @brief Set brightness mode.
	STATE_BRIGHTNESS = 0x300,
	/// @brief Low power, display off mode.
	STATE_LOW_POWER = 0x400,
} State;

static void btn_change(void);
static void display(bool sleep);
static void short_press(void);
static void long_press(void);
static void init(void);

static State state = STATE_SHOW_TIME;

static Digit MENU_DIGITS[] = { DIG_t, DIG_b, DIG_NONE, DIG_NONE };

int main(void)
{
	init();

	while (true) {
		display(true);
	}
}

/// @brief RTC clock tick handler.
void RTC_IRQHandler(void) {
	if (RTC_SR & RTC_SR_TAF_MASK) {
		uint32_t alarm = RTC_TAR;
		uint32_t alarm2 = alarm + TICK_LEN;
		// Detect future overflow
		if (alarm2 < alarm) {
			// Reset the clock when it would overflow
			rtc_reset();
		} else {
			// No overflow
			RTC_TAR = alarm2;
		}
		clock_tick();
	} if (RTC_SR & (RTC_SR_TOF_MASK | RTC_SR_TIF_MASK)) {
		// Shouldn't happen, but if it does, reset the clock. Some precision
		// may be lost
		rtc_reset();
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
		display(false);
		if (NOW - press_time >= 2) {
			long_press();
			return;
		}
	}
	short_press();
}

static void display(bool sleep) {
	switch (state & STATE_PRIMARY) {
		case STATE_SHOW_TIME:
			show_time(1);
			break;
		case STATE_EDIT_TIME:
			show_time(state & STATE_SEC);
			break;
		case STATE_MENU:
			show_digs(DIG_t, DIG_b, DIG_NONE, DIG_NONE, state & STATE_SEC);
			break;
		case STATE_BRIGHTNESS:
			show_digs(
				DIG_b,
				DIG_NONE,
				brightness == 10 ? DIG_1 : DIG_NONE,
				brightness == 10 ? DIG_0 : DIGIT[brightness],
				1
			);
			break;
		case STATE_LOW_POWER:
			if (sleep) {
				__WFI();
			} else {
				show_time(1);
			}
			break;
		default:
			show_digs(DIG_E, DIG_r, DIG_o, DIG_r, ~0);
			break;
	}
}

static void short_press(void) {
	switch (state & STATE_PRIMARY) {
		case STATE_SHOW_TIME:
			state = STATE_LOW_POWER;
			break;
		case STATE_EDIT_TIME:
			++time[state & STATE_SEC];
			clock_truncate();
			break;
		case STATE_MENU_TIME:
			state = state == STATE_MENU_TIME
				? STATE_MENU_BRIGHTNESS
				: STATE_SHOW_TIME;
			break;
		case STATE_BRIGHTNESS:
			++brightness;
			if (brightness > 10) {
				brightness = 0;
			}
			break;
		case STATE_LOW_POWER:
			state = STATE_SHOW_TIME;
			break;
	}
}

static void long_press(void) {
	switch (state) {
		case STATE_SHOW_TIME:
			state = STATE_MENU;
			break;
		case STATE_MENU_TIME:
			rtc_tick(false);
			state = STATE_EDIT_TIME;
			break;
		case STATE_MENU_BRIGHTNESS:
			state = STATE_BRIGHTNESS;
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
		case STATE_BRIGHTNESS:
			state = STATE_SHOW_TIME;
			break;
		case STATE_LOW_POWER:
			state = STATE_SHOW_TIME;
			break;
	}
}

static void init(void) {
	// Disable watchdog
	SIM_COPC = 0;

	gpio_init();
	rtc_init();

	// VLLS3
	SMC_STOPCTRL |= SMC_STOPCTRL_VLLSM(0x3);
	// VLLSx | VLPR
	SMC_PMCTRL = SMC_PMCTRL_STOPM(0x4) | SMC_PMCTRL_RUNM(0x2);
}
