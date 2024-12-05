#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

#include <stddef.h>

#include "MKL05Z4.h"

#include "utility.h"

#define IS_PRESSED (!(GPIOB_PDIR & SEG_BTN))
// Active wait time for each of the displays
#define DISPLAY_DELAY 100
// Maximum brightness
#define MAX_BRIGHTNESS 10

/// @brief Brightness of the display when using `show_msg` and `show_digs`
extern unsigned brightness;

/// @brief Segment names.
typedef enum {
	SEG_A = 1 << 0,
	SEG_B = 1 << 1,
	SEG_C = 1 << 2,
	SEG_D = 1 << 8,
	SEG_E = 1 << 9,
	SEG_F = 1 << 10,
	SEG_G = 1 << 11,
	SEG_DP = 1 << 3,
	SEG_BTN = 1 << 4,
} Segment;

/// @brief Names for useful characters that can be shown on a display.
typedef enum {
	DIG_ALL = 0xf0f,
	DIG_NONE = 0,

	DIG_0 = DIG_ALL & ~SEG_DP & ~SEG_G,
	DIG_1 = SEG_B | SEG_C,
	DIG_2 = DIG_ALL & ~SEG_DP & ~SEG_C & ~SEG_F,
	DIG_3 = DIG_1 | SEG_A | SEG_D | SEG_G,
	DIG_4 = DIG_1 | SEG_F | SEG_G,
	DIG_5 = DIG_ALL & ~SEG_DP & ~SEG_B & ~SEG_E,
	DIG_6 = DIG_ALL & ~SEG_DP & ~SEG_B,
	DIG_7 = SEG_A | SEG_B | SEG_C,
	DIG_8 = DIG_ALL & ~SEG_DP,
	DIG_9 = DIG_8 & ~SEG_E,

	DIG_E = DIG_8 & ~DIG_1,
	DIG_r = SEG_E | SEG_G,
	DIG_o = DIG_6 & ~SEG_A & ~SEG_F,

	DIG_t = DIG_E & ~SEG_A,
	DIG_b = DIG_t | SEG_C,
	DIG_BACK = SEG_G,
} Digit;

/// @brief Display selector.
typedef enum {
	DIS_ALL = 0xf00,
	DIS_NONE = 0,
	DIS_0 = 1 << 11,
	DIS_1 = 1 << 10,
	DIS_2 = 1 << 9,
	DIS_3 = 1 << 8,
} Display;

/// @brief Maps int to display.
static const Display DISPLAY[] = { DIS_0, DIS_1, DIS_2, DIS_3 };
/// @brief Number of displays.
#define DISPLAY_LEN (sizeof(DISPLAY) / sizeof(*DISPLAY))

/// @brief Maps digit to its display character.
static const Digit DIGIT[] = {
	DIG_0, DIG_1, DIG_2, DIG_3, DIG_4, DIG_5, DIG_6, DIG_7, DIG_8, DIG_9
};

/// @brief Initialize the ports.
void gpio_init(void);

/// @brief Show character on the given display.
/// @param val Character to show.
/// @param dis Display to use.
static inline void show(Digit val, Display dis) {
	GPIOA_PSOR = DIS_ALL;
	GPIOB_PCOR = DIG_ALL;
	GPIOA_PCOR = dis;
	GPIOB_PSOR = val;
}

/// @brief Show the current display value with brightness.
static inline void bright_wait() {
	active_wait(brightness * DISPLAY_DELAY);
	show(DIG_NONE, DIS_NONE);
	active_wait((MAX_BRIGHTNESS - brightness) * DISPLAY_DELAY);
}

/// @brief Show the given characters on the display.
/// @param d Characters to show on the display. It is expected to have length 4
/// @param dot Which character should have the dot.
static inline void show_msg(Digit *d, size_t dot) {
	for (size_t i = 0; i < DISPLAY_LEN; ++i) {
		Digit base = i == dot ? SEG_DP : DIG_NONE;
		show(d[i] | base, DISPLAY[i]);
		bright_wait();
	}
}

/// @brief Show characters on the display.
/// @param a First char.
/// @param b Second char.
/// @param c Third char.
/// @param d Fourth and the last char.
/// @param dot Which of the chars should have the dot.
static inline void show_digs(Digit a, Digit b, Digit c, Digit d, size_t dot) {
	Digit msg[] = { a, b, c, d };
	show_msg(msg, dot);
}

#endif // GPIO_H_INCLUDED
