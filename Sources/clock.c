#include "clock.h"

unsigned time[] = { 0, 0, 0, 0 };

void clock_tick(void) {
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

void clock_truncate(void) {
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
