#include <types.h>

uint64_t udiv64(uint64_t n, uint64_t d) {
#if !defined(__amd64__) && !defined(__x86_64__)
    uint64_t q = 0, r = 0;
    for (int i = 63; i >= 0; i--) {
        r = (r << 1) | ((n >> i) & 1);
        if (r >= d) {
            r -= d;
            q |= 1ULL << i;
        }
    }
    return q;
#else
    return n / d;
#endif
}

uint64_t umod64(uint64_t n, uint64_t d) {
#if !defined(__amd64__) && !defined(__x86_64__)
    uint64_t r = 0;
    for (int i = 63; i >= 0; i--) {
        r = (r << 1) | ((n >> i) & 1);
        if (r >= d) r -= d;
    }
    return r;
#else
    return n % d;
#endif
}

kbool is_valid_int(const char* str) {
    if (!str || !str[0])
        return KFALSE;

    int i = 0;

    if (str[0] == '-') {
        if (!str[1])
            return KFALSE;
        if (str[1] == 0 && !str[2]) 
            return KFALSE;
        i = 1;
    }

    for (; str[i]; i++) {
        if (str[i] < '0' || str[i] > '9')
            return KFALSE;
    }

    return KTRUE;
}

int64_t str_to_int64(const char* str) {
    int64_t result = 0;
    int sign = 1;
    int i = 0;

    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }

    for (; str[i]; i++) {
        result = result * 10 + (str[i] - '0');
    }

    return result * sign;
}
