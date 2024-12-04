/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <string.h>
#include <screen.h>

void printprefix(const char* user, const char* pcname) {
    printstr(user, 0x0B);
    printstr("@", 0x0F);
    printstr(pcname, 0x02);
    printstr(" $ ", 0x0F);
}