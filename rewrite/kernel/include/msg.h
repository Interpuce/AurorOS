/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>

typedef enum {
    CodeLogTypeOK,
    CodeLogTypeInfo,
    CodeLogTypeWarning,
    CodeLogTypeError
} LogTypeCodes;

extern void printStatus(LogTypeCodes type, string message);