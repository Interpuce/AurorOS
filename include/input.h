/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#ifndef KB_H
#define KB_H
#include <stdbool.h>
#include "screen.h"
#include "system.h"
#include "types.h"

string read_string(int8 colr) {
    char buff;
    bool input_shift = false;
    string buffstr;
    uint8 i = 0;
    uint8 reading = 1;

    while (reading) {
    if (inb(0x64) & 0x1) {
        uint8 keycode = inb(0x60);

        if (keycode == 42 || keycode == 54) {
        input_shift = true;
        continue;
        } else if (keycode == 170 || keycode == 182) {
        input_shift = false;
        continue;
        }

        switch (keycode) {

        case 1:
            printch((char)27,colr);
            buffstr[i] = (char)27;
            i++;
            break;
        case 2:
            printch(input_shift ? '!' : '1', colr);
            buffstr[i] = input_shift ? '!' : '1'; 
            i++;
            break;
        case 3:
            printch(input_shift ? '@' : '2', colr);
            buffstr[i] = input_shift ? '@' : '2';
            i++;
            break;
        case 4:
            printch(input_shift ? '#' : '3', colr);
            buffstr[i] = input_shift ? '#' : '3';
            i++;
            break;  
        case 5:
            printch(input_shift ? '$' : '4', colr);
            buffstr[i] = input_shift ? '$' : '4';
            i++;
            break;  
        case 6:
            printch(input_shift ? '%' : '5', colr);
            buffstr[i] = input_shift ? '%' : '5';
            i++;
            break;  
        case 7:
            printch(input_shift ? '^' : '6', colr);
            buffstr[i] = input_shift ? '^' : '6';
            i++;
            break;  
        case 8:
            printch(input_shift ? '&' : '7', colr);
            buffstr[i] = input_shift ? '&' : '7';
            i++;
            break;  
        case 9:
            printch(input_shift ? '*' : '8', colr);
            buffstr[i] = input_shift ? '*' : '8';
            i++;
            break;  
        case 10:
            printch(input_shift ? '(' : '9', colr);
            buffstr[i] = input_shift ? '(' : '9';
            i++;
            break;  
        case 11:
            printch(input_shift ? ')' : '0', colr);
            buffstr[i] = input_shift ? ')' : '0';
            i++;
            break;  
        case 12:
            printch(input_shift ? '_' : '-', colr);
            buffstr[i] = input_shift ? '_' : '-';
            i++;
            break;  
        case 13:
            printch(input_shift ? '+' : '=', colr);
            buffstr[i] = input_shift ? '+' : '=';
            i++;
            break;  
        case 14:
            printch('\b',colr);
            i--;
            buffstr[i] = 0;
            break;
        case 15:
            printch('\t',colr);
            buffstr[i] = '\t';
            i++;
            break;
        case 16:
            printch(input_shift ? 'Q' : 'q', colr);
            buffstr[i] = input_shift ? 'Q' : 'q';
            i++;
            break;
        case 17:
            printch(input_shift ? 'W' : 'w', colr);
            buffstr[i] = input_shift ? 'W' : 'w';
            i++;
            break;
        case 18:
            printch(input_shift ? 'E' : 'e', colr);
            buffstr[i] = input_shift ? 'E' : 'e';
            i++;
            break;
        case 19:
            printch(input_shift ? 'R' : 'r', colr);
            buffstr[i] = input_shift ? 'R' : 'r';
            i++;
            break;
        case 20:
            printch(input_shift ? 'T' : 't', colr);
            buffstr[i] = input_shift ? 'T' : 't';
            i++;
            break;
        case 21:
            printch(input_shift ? 'Y' : 'y', colr);
            buffstr[i] = input_shift ? 'Y' : 'y';
            i++;
            break;
        case 22:
            printch(input_shift ? 'U' : 'u', colr);
            buffstr[i] = input_shift ? 'U' : 'u';
            i++;
            break;
        case 23:
            printch(input_shift ? 'I' : 'i', colr);
            buffstr[i] = input_shift ? 'I' : 'i';
            i++;
            break;
        case 24:
            printch(input_shift ? 'O' : 'o', colr);
            buffstr[i] = input_shift ? 'O' : 'o';
            i++;
            break;
        case 25:
            printch(input_shift ? 'P' : 'p', colr);
            buffstr[i] = input_shift ? 'P' : 'p';
            i++;
            break;
        case 26:
            printch(input_shift ? '{' : '[', colr);
            buffstr[i] = input_shift ? '{' : '[';
            i++;
            break;
        case 27:
            printch(input_shift ? '}' : ']', colr);
            buffstr[i] = input_shift ? '}' : ']';
            i++;
            break;
        case 28:
            i++;
            reading = 0;
            break;
        case 29:
            printch(input_shift ? 'Q' : 'q', colr);
            buffstr[i] = input_shift ? 'Q' : 'q';
            i++;
            break;
        case 30:
            printch(input_shift ? 'A' : 'a', colr);
            buffstr[i] = input_shift ? 'A' : 'a';
            i++;
            break;
        case 31:
            printch(input_shift ? 'S' : 's', colr);
            buffstr[i] = input_shift ? 'S' : 's';
            i++;
            break;
        case 32:
            printch(input_shift ? 'D' : 'd', colr);
            buffstr[i] = input_shift ? 'D' : 'd';
            i++;
            break;
        case 33:
            printch(input_shift ? 'F' : 'f', colr);
            buffstr[i] = input_shift ? 'F' : 'f';
            i++;
            break;
        case 34:
            printch(input_shift ? 'G' : 'g', colr);
            buffstr[i] = input_shift ? 'G' : 'g';
            i++;
            break;
        case 35:
            printch(input_shift ? 'H' : 'h', colr);
            buffstr[i] = input_shift ? 'H' : 'h';
            i++;
            break;
        case 36:
            printch(input_shift ? 'J' : 'j', colr);
            buffstr[i] = input_shift ? 'J' : 'j';
            i++;
            break;
        case 37:
            printch(input_shift ? 'K' : 'k', colr);
            buffstr[i] = input_shift ? 'K' : 'k';
            i++;
            break;
        case 38:
            printch(input_shift ? 'L' : 'l', colr);
            buffstr[i] = input_shift ? 'L' : 'l';
            i++;
            break;
        case 39:
            printch(input_shift ? ':' : ';', colr);
            buffstr[i] = input_shift ? ':' : ';';
            i++;
            break;
        case 40:
            printch((char)44,colr);
            buffstr[i] = (char)44;
            i++;
            break;
        case 41:
            printch((char)44,colr);
            buffstr[i] = (char)44;
            i++;
            break;
        case 42:
            input_shift = true;
            break;
        case 43:
            printch((char)92,colr);
            buffstr[i] = 'q';
            i++;
            break;
        case 44:
            printch(input_shift ? 'Z' : 'z', colr);
            buffstr[i] = input_shift ? 'Z' : 'z';
            i++;
            break;
        case 45:
            printch(input_shift ? 'X' : 'x', colr);
            buffstr[i] = input_shift ? 'X' : 'x';
            i++;
            break;
        case 46:
            printch(input_shift ? 'C' : 'c', colr);
            buffstr[i] = input_shift ? 'C' : 'c';
            i++;
            break;
        case 47:
            printch(input_shift ? 'V' : 'v', colr);
            buffstr[i] = input_shift ? 'V' : 'v';
            i++;
            break;          
        case 48:
            printch(input_shift ? 'B' : 'b', colr);
            buffstr[i] = input_shift ? 'B' : 'b';
            i++;
            break;          
        case 49:
            printch(input_shift ? 'N' : 'n', colr);
            buffstr[i] = input_shift ? 'N' : 'n';
            i++;
            break;     
        case 50:
            printch(input_shift ? 'M' : 'm', colr);
            buffstr[i] = input_shift ? 'M' : 'm';
            i++;
            break;         
        case 51:
            printch(input_shift ? '<' : ',', colr);
            buffstr[i] = input_shift ? '<' : ',';
            i++;
            break;        
        case 52:
            printch(input_shift ? '>' : '.', colr);
            buffstr[i] = input_shift ? '>' : '.';
            i++;
            break;      
        case 53:
            printch(input_shift ? '?' : '/', colr);
            buffstr[i] = input_shift ? '?' : '/';
            i++;
            break;         
        case 54:
            printch('.',colr);
            buffstr[i] = '.';
            i++;
            break;        
        case 55:
            printch('/',colr);
            buffstr[i] = '/';
            i++;
            break;        
        case 56:
            printch(' ',colr);
            buffstr[i] = ' ';
            i++;
            break;       
        case 57:
            printch(' ',colr);
            buffstr[i] = ' ';
            i++;
            break;

        case 170:
            case 182:
            input_shift = false;
            break;
        }
        }

        
    }
    buffstr[i] = 0;           
    return buffstr;
}


string invisible_read_string() {
    char buff;
    bool input_shift = false;
    string buffstr;
    uint8 i = 0;
    uint8 reading = 1;

    while (reading) {
    if (inb(0x64) & 0x1) {
        uint8 keycode = inb(0x60);

        if (keycode == 42 || keycode == 54) {
            input_shift = true;
            continue;
        } else if (keycode == 170 || keycode == 182) {
            input_shift = false;
            continue;
        }

        switch (keycode) {

        case 1:
            buffstr[i] = (char)27;
            i++;
            break;
        case 2:
            buffstr[i] = input_shift ? '!' : '1'; 
            i++;
            break;
        case 3:
            buffstr[i] = input_shift ? '@' : '2';
            i++;
            break;
        case 4:
            buffstr[i] = input_shift ? '#' : '3';
            i++;
            break;  
        case 5:
            buffstr[i] = input_shift ? '$' : '4';
            i++;
            break;  
        case 6:
            buffstr[i] = input_shift ? '%' : '5';
            i++;
            break;  
        case 7:
            buffstr[i] = input_shift ? '^' : '6';
            i++;
            break;  
        case 8:
            buffstr[i] = input_shift ? '&' : '7';
            i++;
            break;  
        case 9:
            buffstr[i] = input_shift ? '*' : '8';
            i++;
            break;  
        case 10:
            buffstr[i] = input_shift ? '(' : '9';
            i++;
            break;  
        case 11:
            buffstr[i] = input_shift ? ')' : '0';
            i++;
            break;  
        case 12:
            buffstr[i] = input_shift ? '_' : '-';
            i++;
            break;  
        case 13:
            buffstr[i] = input_shift ? '+' : '=';
            i++;
            break;  
        case 14:
            i--;
            buffstr[i] = 0;
            break;
        case 15:
            buffstr[i] = '\t';
            i++;
            break;
        case 16:
            buffstr[i] = input_shift ? 'Q' : 'q';
            i++;
            break;
        case 17:
            buffstr[i] = input_shift ? 'W' : 'w';
            i++;
            break;
        case 18:
            buffstr[i] = input_shift ? 'E' : 'e';
            i++;
            break;
        case 19:
            buffstr[i] = input_shift ? 'R' : 'r';
            i++;
            break;
        case 20:
            buffstr[i] = input_shift ? 'T' : 't';
            i++;
            break;
        case 21:
            buffstr[i] = input_shift ? 'Y' : 'y';
            i++;
            break;
        case 22:
            buffstr[i] = input_shift ? 'U' : 'u';
            i++;
            break;
        case 23:
            buffstr[i] = input_shift ? 'I' : 'i';
            i++;
            break;
        case 24:
            buffstr[i] = input_shift ? 'O' : 'o';
            i++;
            break;
        case 25:
            buffstr[i] = input_shift ? 'P' : 'p';
            i++;
            break;
        case 26:
            buffstr[i] = input_shift ? '{' : '[';
            i++;
            break;
        case 27:
            buffstr[i] = input_shift ? '}' : ']';
            i++;
            break;
        case 28:
            i++;
            reading = 0;
            break;
        case 29:
            buffstr[i] = input_shift ? 'Q' : 'q';
            i++;
            break;
        case 30:
            buffstr[i] = input_shift ? 'A' : 'a';
            i++;
            break;
        case 31:
            buffstr[i] = input_shift ? 'S' : 's';
            i++;
            break;
        case 32:
            buffstr[i] = input_shift ? 'D' : 'd';
            i++;
            break;
        case 33:
            buffstr[i] = input_shift ? 'F' : 'f';
            i++;
            break;
        case 34:
            buffstr[i] = input_shift ? 'G' : 'g';
            i++;
            break;
        case 35:
            buffstr[i] = input_shift ? 'H' : 'h';
            i++;
            break;
        case 36:
            buffstr[i] = input_shift ? 'J' : 'j';
            i++;
            break;
        case 37:
            buffstr[i] = input_shift ? 'K' : 'k';
            i++;
            break;
        case 38:
            buffstr[i] = input_shift ? 'L' : 'l';
            i++;
            break;
        case 39:
            buffstr[i] = input_shift ? ':' : ';';
            i++;
            break;
        case 40:
            buffstr[i] = (char)44;
            i++;
            break;
        case 41:
            buffstr[i] = (char)44;
            i++;
            break;
        case 42:
            input_shift = true;
            break;
        case 43:
            buffstr[i] = 'q';
            i++;
            break;
        case 44:
            buffstr[i] = input_shift ? 'Z' : 'z';
            i++;
            break;
        case 45:
            buffstr[i] = input_shift ? 'X' : 'x';
            i++;
            break;
        case 46:
            buffstr[i] = input_shift ? 'C' : 'c';
            i++;
            break;
        case 47:
            buffstr[i] = input_shift ? 'V' : 'v';
            i++;
            break;          
        case 48:
            buffstr[i] = input_shift ? 'B' : 'b';
            i++;
            break;          
        case 49:
            buffstr[i] = input_shift ? 'N' : 'n';
            i++;
            break;     
        case 50:
            buffstr[i] = input_shift ? 'M' : 'm';
            i++;
            break;         
        case 51:
            buffstr[i] = input_shift ? '<' : ',';
            i++;
            break;        
        case 52:
            buffstr[i] = input_shift ? '>' : '.';
            i++;
            break;      
        case 53:
            buffstr[i] = input_shift ? '?' : '/';
            i++;
            break;         
        case 54:
            buffstr[i] = '.';
            i++;
            break;        
        case 55:
            buffstr[i] = '/';
            i++;
            break;        
        case 56:
            buffstr[i] = ' ';
            i++;
            break;       
        case 57:
            buffstr[i] = ' ';
            i++;
            break;

        case 170:
            case 182:
            input_shift = false;
            break;
        }
        }
    }
    buffstr[i] = 0;           
    return buffstr;
}

#endif