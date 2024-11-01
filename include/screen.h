#ifndef SCREEN_H
#define SCREEN_H
#include "types.h"
#include "system.h"
#include "string.h"
int cursorX = 0, cursorY = 0;
const uint8 sw = 80,sh = 25,sd = 2;
void clear_line(uint8 from,uint8 to)
{
    uint16 i = sw * from * sd;
    string vidmem=(string)0xb8000;
    for(i;i<(sw*to*sd);i++)
    {
        vidmem[i] = 0x0;
    }
}
void update_cursor()
{
    unsigned temp;

    temp = cursorY * sw + cursorX;

    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temp);
}
void clear_console()
{
    clear_line(0,sh-1);
    cursorX = 0;
    cursorY = 0;
    update_cursor();
}

void scroll_up(uint8 lineNumber)
{
    string vidmem = (string)0xb8000;
    uint16 i = 0;
    clear_line(0,lineNumber-1);
    for (i;i<sw*(sh-1)*2;i++)
    {
        vidmem[i] = vidmem[i+sw*2*lineNumber];
    }
    clear_line(sh-1-lineNumber,sh-1);
    if((cursorY - lineNumber) < 0 ) 
    {
        cursorY = 0;
        cursorX = 0;
    } 
    else 
    {
        cursorY -= lineNumber;
    }
    update_cursor();
}


void nl_check()
{
    if(cursorY >=sh-1)
    {
        scroll_up(1);
    }
}

void printch(char c, int8 color)
{
    string vidmem = (string) 0xb8000;     
    switch(c)
    {
    case (0x08):
        if(cursorX > 0) 
        {
	        cursorX--;									
            vidmem[(cursorY * sw + cursorX)*sd]=0x00;	                  
	    }
	    break;
    case ('\r'):
        cursorX = 0;
        break;
    case ('\n'):
        cursorX = 0;
        cursorY++;
        break;
    default:
        vidmem [((cursorY * sw + cursorX))*sd] = c;
        vidmem [((cursorY * sw + cursorX))*sd+1] = color;
        cursorX++; 
        break;
	
    }
    if(cursorX >= sw)                                   
    {
    cursorX = 0;                                
    cursorY++;                                    
    }
    update_cursor();
    nl_check();
}

void print(string ch, int8 clr)
{
    uint32 i = 0;
    uint32 length = strlength(ch)-1;
    for(i;i<length;i++)
    {
        printch(ch[i], clr);
    }
}

#endif
