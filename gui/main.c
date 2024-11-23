/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include <console.h>
#include <string.h>

int start_gui(const string user) {
    if (!string_equal(user, "main")) {
        kernel_panic("NOT_ENOUGH_GUI_PERMISSIONS");
        return 0;
    }
}