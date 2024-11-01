/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include "../include/screen.h"
#include "../include/input.h"
#include "../include/string.h"

void main()
{
       clear_console();
       print("Welcome to aurorOS!\n", 0x07);
       while (1)
       {
                print("\nroot@aurorOS > ", 0x05);
                
                string ch = read_string(0x07);
                if(string_equal(ch,"ver"))
                {
                        print("\nAurorOS beta sigma alpha domino pizza skibidi toilet\n", 0x07);
                }
                else if(string_equal(ch,"clear"))
                {
                        clear_console();
                }
                else
                {
                        print("\nInvalid command!\n", 0x04);
                }
                
                print("\n", 0x07);        
       }
        
}
