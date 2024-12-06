#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

static inline void active_wait(unsigned long len) {
    // Make sure this works when `len` is 0
    ++len;
    while (--len) { }
}

#endif // UTILITY_H_INCLUDED
