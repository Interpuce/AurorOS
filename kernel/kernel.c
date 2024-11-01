#include "../include/screen.h"
#include "../include/input.h"
#include "../include/string.h"

void main()
{
       clearScreen();
       print("Welcome to aurorOS!\n", 0x07);
       while (1)
       {
                print("\nroot@aurorOS > ", 0x05);
                
                string ch = readStr(0x07);
                if(string_equal(ch,"ver"))
                {
                        print("\nAurorOS beta sigma alpha domino pizza skibidi toilet\n", 0x07);
                }
                else if(string_equal(ch,"clear"))
                {
                        clearScreen();
                }
                else
                {
                        print("\nInvalid command!\n", 0x04);
                }
                
                print("\n", 0x07);        
       }
        
}
