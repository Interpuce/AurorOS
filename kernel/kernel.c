/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <screen.h>
#include <input.h>
#include <string.h>

void main()
{
    clear_console();
    print("Welcome to AurorOS!\n", 0x07);
    while (1)
    {
        print("\nroot@AurorOS > ", 0x05);
  
        string ch = read_string(0x07);
        int count;
        string* charray = split_by_spaces(ch, count);

        if (string_equal(charray[0], 'ver')) {
            print("AurorOS 1.0.0", 0x07);
            print("This version is very unstable and may contain errors and bugs. Let us know on dsc.gg/Auror-OS", 0x04);
        } else {
            print("Invalid command!", 0x04);
        }
    }  
}
