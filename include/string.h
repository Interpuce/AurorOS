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

#define MAX_WORDS 100  // if someone knows how to do this without max_words make a pull request

string* split_by_spaces(string ch, uint16* count) {
    static string result[MAX_WORDS];
    uint16 index = 0;
    uint16 i = 0;

    if (ch[0] == '\0') {
        result[0] = ch;
        *count = 1;
        return result;
    }

    result[index++] = ch;

    while (ch[i] != '\0') {
        if (ch[i] == ' ') {
            ch[i] = '\0';

            if (ch[i + 1] != '\0' && ch[i + 1] != ' ' && index < MAX_WORDS) {
                result[index++] = &ch[i + 1];
            }
        }
        i++;
    }

    *count = index;
    return result;
}

#endif
