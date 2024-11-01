#ifndef KB_H
#define KB_H
#include "screen.h"
#include "system.h"
#include "types.h"

string read_string(int8 colr)
{
    char buff;
    string buffstr;
    uint8 i = 0;
    uint8 reading = 1;
    while(reading)
    {
    if(inb(0x64) & 0x1)         
    {
        switch(inb(0x60))
        { 
    case 1:
        printch((char)27,colr);
        buffstr[i] = (char)27;
        i++;
        break;
    case 2:
        printch('1',colr);
        buffstr[i] = '1';
        i++;
        break;
    case 3:
        printch('2',colr);
        buffstr[i] = '2';
        i++;
        break;
    case 4:
        printch('3',colr);
        buffstr[i] = '3';
        i++;
        break;
    case 5:
        printch('4',colr);
        buffstr[i] = '4';
        i++;
        break;
    case 6:
        printch('5',colr);
        buffstr[i] = '5';
        i++;
        break;
    case 7:
        printch('6',colr);
        buffstr[i] = '6';
        i++;
        break;
    case 8:
        printch('7',colr);
        buffstr[i] = '7';
        i++;
        break;
    case 9:
        printch('8',colr);
        buffstr[i] = '8';
        i++;
        break;
    case 10:
        printch('9',colr);
        buffstr[i] = '9';
        i++;
        break;
    case 11:
        printch('0',colr);
        buffstr[i] = '0';
        i++;
        break;
    case 12:
        printch('-',colr);
        buffstr[i] = '-';
        i++;
        break;
    case 13:
        printch('=',colr);
        buffstr[i] = '=';
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
        printch('q',colr);
        buffstr[i] = 'q';
        i++;
        break;
    case 17:
        printch('w',colr);
        buffstr[i] = 'w';
        i++;
        break;
    case 18:
        printch('e',colr);
        buffstr[i] = 'e';
        i++;
        break;
    case 19:
        printch('r',colr);
        buffstr[i] = 'r';
        i++;
        break;
    case 20:
        printch('t',colr);
        buffstr[i] = 't';
        i++;
        break;
    case 21:
        printch('y',colr);
        buffstr[i] = 'y';
        i++;
        break;
    case 22:
        printch('u',colr);
        buffstr[i] = 'u';
        i++;
        break;
    case 23:
        printch('i',colr);
        buffstr[i] = 'i';
        i++;
        break;
    case 24:
        printch('o',colr);
        buffstr[i] = 'o';
        i++;
        break;
    case 25:
        printch('p',colr);
        buffstr[i] = 'p';
        i++;
        break;
    case 26:
        printch('[',colr);
        buffstr[i] = '[';
        i++;
        break;
    case 27:
        printch(']',colr);
        buffstr[i] = ']';
        i++;
        break;
    case 28:
        i++;
        reading = 0;
        break;
    case 29:
        printch('q',colr);
        buffstr[i] = 'q';
        i++;
        break;
    case 30:
        printch('a',colr);
        buffstr[i] = 'a';
        i++;
        break;
    case 31:
        printch('s',colr);
        buffstr[i] = 's';
        i++;
        break;
    case 32:
        printch('d',colr);
        buffstr[i] = 'd';
        i++;
        break;
    case 33:
        printch('f',colr);
        buffstr[i] = 'f';
        i++;
        break;
    case 34:
        printch('g',colr);
        buffstr[i] = 'g';
        i++;
        break;
    case 35:
        printch('h',colr);
        buffstr[i] = 'h';
        i++;
        break;
    case 36:
        printch('j',colr);
        buffstr[i] = 'j';
        i++;
        break;
    case 37:
        printch('k',colr);
        buffstr[i] = 'k';
        i++;
        break;
    case 38:
        printch('l',colr);
        buffstr[i] = 'l';
        i++;
        break;
    case 39:
        printch(';',colr);
        buffstr[i] = ';';
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
        printch('q',colr);
        buffstr[i] = 'q';
        i++;
        break;
    case 43:
        printch((char)92,colr);
        buffstr[i] = 'q';
        i++;
        break;
    case 44:
        printch('z',colr);
        buffstr[i] = 'z';
        i++;
        break;
    case 45:
        printch('x',colr);
        buffstr[i] = 'x';
        i++;
        break;
    case 46:
        printch('c',colr);
        buffstr[i] = 'c';
        i++;
        break;
    case 47:
        printch('v',colr);
        buffstr[i] = 'v';
        i++;
        break;        
    case 48:
        printch('b',colr);
        buffstr[i] = 'b';
        i++;
        break;           
    case 49:
        printch('n',colr);
        buffstr[i] = 'n';
        i++;
        break;        
    case 50:
        printch('m',colr);
        buffstr[i] = 'm';
        i++;
        break;           
    case 51:
        printch(',',colr);
        buffstr[i] = ',';
        i++;
        break;        
    case 52:
        printch('.',colr);
        buffstr[i] = '.';
        i++;
        break;        
    case 53:
        printch('/',colr);
        buffstr[i] = '/';
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
        }
    }
    }
    buffstr[i] = 0;           
    return buffstr;
}
#endif
