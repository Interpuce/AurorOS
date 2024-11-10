/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#ifndef STRING_H
#define STRING_H

#include <types.h>
uint16 strlength(string ch)
{
    uint16 i = 1;
    while(ch[i++]);  
    return --i;
}

uint8 string_equal(string ch1,string ch2)             
{
    uint32 result = 1;
    uint32 size = strlength(ch1);
    if(size != strlength(ch2)) result =0;
    else 
    {
        uint32 i = 0;
        for(i;i<=size;i++)
        {
            if(ch1[i] != ch2[i]) result = 0;
        }
    }
    return result;
}

#endif
