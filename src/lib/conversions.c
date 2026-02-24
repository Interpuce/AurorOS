#include <numbers.h>

char* utoa(uint64_t value, char* buffer, int base) {
    static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char* ptr = buffer;
    char* ptr1 = buffer;
    char tmp_char;

    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return buffer;
    }

    while (value != 0) {
        uint64_t remainder = umod64(value, base);
        *ptr++ = digits[remainder];
        value = udiv64(value, base);
    }
    *ptr = '\0';

    ptr--;
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }

    return buffer;
}
