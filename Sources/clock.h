#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED

#include <stddef.h>

#include "gpio.h"

extern unsigned time[];

// Hours high (first digit)
#define HH (time[0])
// Hours low (second digit)
#define HL (time[1])
// Minutes high (first digit)
#define MH (time[2])
// Minutes low (second digit)
#define ML (time[3])

void clock_tick(void);
void clock_truncate(void);

static void show_time(size_t dot) {
	for (size_t i = 0; i < DISPLAY_LEN; ++i) {
		Digit base = i == dot ? SEG_DP : DIG_NONE;
		show(DIGIT[time[i]] | base, DISPLAY[i]);
		bright_wait();
	}
}

#endif // CLOCK_H_INCLUDED
