#include <types.h>

extern uint64_t udiv64(uint64_t n, uint64_t d);
extern uint64_t umod64(uint64_t n, uint64_t d);

extern char* utoa(uint64_t value, char* buffer, int base);

extern int64_t str_to_int64(const char* str);
extern kbool is_valid_int(const char* str);
