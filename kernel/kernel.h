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
#include <msg.h>

void printprefix(const char* user, const char* pcname) {
    print(" [ ", 0x07);
    print(user, 0x01);
    print("@", 0x0F);
    print(pcname, 0x02);
    print(" ]", 0x07);
    print(" $ ", 0x0F);
}
