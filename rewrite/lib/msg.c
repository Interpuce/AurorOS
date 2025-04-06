/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <msg.h>
#include <drivers/screen.h>

void print_status(LogTypeCodes type, string message) {
    uint8 color;
    string type_string;
    switch (type) {
        case CodeLogTypeOK:
            color = 0x02;
            type_string = 'OK';
            break;

        case CodeLogTypeInfo:
            color = 0x01;
            type_string = 'INFO';
            break;

        case CodeLogTypeWarning:
            color = 0x06;
            type_string = 'WARN';
            break;

        case CodeLogTypeError:
            color = 0x04;
            type_string = 'ERROR';
            break;
        
        default:
            break;
    }

    print(' [ ', 0x07);
    print(type_string, color);
    print(' ] ', 0x07);
    println(message, 0x07);
}