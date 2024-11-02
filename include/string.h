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

string* split_by_spaces(string ch, uint16* count) {
    uint16 i = 0, j = 0, word_count = 1;
    for (i = 0; ch[i] != '\0'; i++) {
        if (ch[i] == ' ') word_count++;
    }

    string* result = (string*)malloc(word_count * sizeof(string));
    uint16 start = 0, index = 0;

    for (i = 0; ch[i] != '\0'; i++) {
        if (ch[i] == ' ' || ch[i + 1] == '\0') {
            uint16 length = i - start + (ch[i + 1] == '\0' ? 1 : 0);
            result[index] = (string)malloc((length + 1) * sizeof(char));

            for (j = 0; j < length; j++) {
                result[index][j] = ch[start + j];
            }
            result[index][j] = '\0';
            index++;
            start = i + 1;
        }
    }
    *count = word_count;
    return result;
}


#endif
