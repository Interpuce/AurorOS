#include <string.h>
#include <screen.h>

void printprefix(const char *user, const char *pcname) {
    printstr(user, 0x0B);
    printstr("@", 0x0F);
    printstr(pcname, 0x02);
    printstr(" $ ", 0x0F);
}