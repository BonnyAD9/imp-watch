#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED

#include <stddef.h>

#include "gpio.h"

/// @brief Time digits that are displayed.
extern unsigned time[];

// Hours high (first digit)
#define HH (time[0])
// Hours low (second digit)
#define HL (time[1])
// Minutes high (first digit)
#define MH (time[2])
// Minutes low (second digit)
#define ML (time[3])

/// @brief Tick the clock.
void clock_tick(void);

/// @brief Truncate the clock to valid values.
void clock_truncate(void);

/// @brief Show time on the display.
/// @param dot Which of the time values has the dot.
static void show_time(size_t dot) {
	for (size_t i = 0; i < DISPLAY_LEN; ++i) {
		Digit base = i == dot ? SEG_DP : DIG_NONE;
		show(DIGIT[time[i]] | base, DISPLAY[i]);
		bright_wait();
	}
}

#endif // CLOCK_H_INCLUDED
