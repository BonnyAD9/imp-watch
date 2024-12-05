#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

#include <stdbool.h>

#include "MKL05Z4.h"

// How often the clock will tick (in seconds)
#define TICK_LEN 60
#define NOW RTC_TSR

/// @brief Enable/disable rtc ticking.
/// @param v true if it should be enabled, otherwise false.
static inline void rtc_tick(bool v) {
    if (v) {
        NVIC_ClearPendingIRQ(RTC_IRQn);
        NVIC_EnableIRQ(RTC_IRQn);
    } else {
        NVIC_DisableIRQ(RTC_IRQn);
    }
}

/// @brief Reset the clock.
static inline void rtc_reset() {
    RTC_TAR = TICK_LEN - 1;
    RTC_SR &= ~RTC_SR_TCE_MASK;
    RTC_TSR = 0;
    RTC_SR |= RTC_SR_TCE_MASK;
}

/// @brief Initialize and start the clock.
void rtc_init(void);

#endif // RTC_H_INCLUDED
