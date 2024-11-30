#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

#include "MKL05Z4.h"

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

#define IS_PRESSED (!(GPIOB_PDIR & SEG_BTN))

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

typedef enum {
	DIS_ALL = 0xf00,
	DIS_NONE = 0,
	DIS_0 = 1 << 11,
	DIS_1 = 1 << 10,
	DIS_2 = 1 << 9,
	DIS_3 = 1 << 8,
} Display;

static const Display DISPLAY[] = { DIS_0, DIS_1, DIS_2, DIS_3 };
#define DISPLAY_LEN (sizeof(DISPLAY) / sizeof(*DISPLAY))

static const Digit DIGIT[] = {
	DIG_0, DIG_1, DIG_2, DIG_3, DIG_4, DIG_5, DIG_6, DIG_7, DIG_8, DIG_9
};

void gpio_init(void);

static inline void show(Digit val, Display dis) {
	GPIOA_PSOR = DIS_ALL;
	GPIOB_PCOR = DIG_ALL;
	GPIOA_PCOR = dis;
	GPIOB_PSOR = val;
}

#endif // GPIO_H_INCLUDED
