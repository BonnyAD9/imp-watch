#include <stdnoreturn.h>

#include "MKL05Z4.h"

#include "ports.h"

static inline void active_wait(unsigned long len) { while (--len) { } }
noreturn void endless_error(void);

int main(void)
{
	init_ports();

    // Shouldn't get here. Show error. Never leave main.
	endless_error();
}

noreturn void endless_error(void) {
	enum { WAIT_TIME = 1000 };
    while (1) {
		show(DIG_E, DIS_0);
		active_wait(WAIT_TIME);
		show(DIG_r, DIS_1);
		active_wait(WAIT_TIME);
		show(DIG_o, DIS_2);
		active_wait(WAIT_TIME);
		show(DIG_r, DIS_3);
		active_wait(WAIT_TIME);
    }
}
