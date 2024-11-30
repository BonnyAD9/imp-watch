#include "rtc.h"

#include "MKL05Z4.h"

#include "utility.h"

void rtc_init(void) {
    // Enable RTC
    SIM_SCGC6 = SIM_SCGC6_RTC_MASK;

    // Reset the clock
    RTC_CR |= RTC_CR_SWR_MASK;
    RTC_CR &= ~RTC_CR_SWR_MASK;

    // Enable oscilator and wait for it to start
    RTC_CR |= RTC_CR_OSCE_MASK;

    // Wait is not necessary, the clock is just a little slower at the start.
    //active_wait(0x500000);

    // Set alarm for every 60 seconds
    RTC_TAR = TICK_LEN;

    // Initial time is 0
    RTC_TSR = 0;

    // Enable interupts on alarm
    NVIC_ClearPendingIRQ(RTC_IRQn);
    NVIC_EnableIRQ(RTC_IRQn);

    // Enable the clock
    RTC_SR |= RTC_SR_TCE_MASK;
}
