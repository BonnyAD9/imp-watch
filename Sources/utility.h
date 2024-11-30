#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

static inline void active_wait(unsigned long len) {
    ++len;
    while (--len) { }
}

#endif // UTILITY_H_INCLUDED
