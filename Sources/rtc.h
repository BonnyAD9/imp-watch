#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

#include <stdbool.h>

#include "MKL05Z4.h"

// How often the clock will tick (in seconds)
#define TICK_LEN 60
#define NOW RTC_TSR

static inline void rtc_tick(bool v) {
    if (v) {
        NVIC_ClearPendingIRQ(RTC_IRQn);
        NVIC_EnableIRQ(RTC_IRQn);
    } else {
        NVIC_DisableIRQ(RTC_IRQn);
    }
}

void rtc_init(void);

#endif // RTC_H_INCLUDED
