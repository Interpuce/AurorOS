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
#include <types.h>

int snprintf(char* buffer, size_t size, const char* format, ...) {
    char* ptr = (char*)&format + sizeof(char*);
    int written = 0;

    for (int i = 0; format[i] != '\0'; i++) {
        if (written >= size - 1) break; 

        if (format[i] == '%' && format[i + 1] == 'd') {
            int num = *(int*)ptr;
            ptr += sizeof(int);

            char num_str[20];
            int j = 0, negative = 0;
            if (num < 0) {
                negative = 1;
                num = -num;
            }

            do {
                num_str[j++] = '0' + (num % 10);
                num /= 10;
            } while (num > 0);
            
            if (negative) {
                num_str[j++] = '-';
            }

            for (int k = 0; k < j / 2; k++) {
                char temp = num_str[k];
                num_str[k] = num_str[j - k - 1];
                num_str[j - k - 1] = temp;
            }

            for (int k = 0; k < j && written < size - 1; k++) {
                buffer[written++] = num_str[k];
            }
            i++;
        } else if (format[i] == '%' && format[i + 1] == 'x') {
            int num = *(int*)ptr;
            ptr += sizeof(int);

            char num_str[20];
            int j = 0;
            
            do {
                int digit = num % 16;
                num_str[j++] = (digit < 10) ? '0' + digit : 'a' + (digit - 10);
                num /= 16;
            } while (num > 0);

            for (int k = 0; k < j / 2; k++) {
                char temp = num_str[k];
                num_str[k] = num_str[j - k - 1];
                num_str[j - k - 1] = temp;
            }

            for (int k = 0; k < j && written < size - 1; k++) {
                buffer[written++] = num_str[k];
            }
            i++;
        } else if (format[i] == '%' && format[i + 1] == 's') {
            char* str = *(char**)ptr;
            ptr += sizeof(char);

            while (*str != '\0' && written < size - 1) {
                buffer[written++] = *str++;
            }
            i++;
        } else {
            buffer[written++] = format[i];
        }
    }

    buffer[written] = '\0';
    
    return written;
}

void printf(const char* format, ...) {
    char* ptr = (char*)&format + sizeof(char*);

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && format[i + 1] == 's') {
            char* str = *(char**)ptr;
            print(str, 0x07);
            ptr += sizeof(char*);
            i++;
        } else if (format[i] == '%' && format[i + 1] == 'd') {
            int num = *(int*)ptr;
            char buffer[20];
            snprintf(buffer, sizeof(buffer), "%d", num);
            print(buffer, 0x07);
            ptr += sizeof(int);
            i++;
        } else if (format[i] == '%' && format[i + 1] == 'u') {
            unsigned int num = *(unsigned int*)ptr;
            char buffer[20];
            snprintf(buffer, sizeof(buffer), "%u", num);
            print(buffer, 0x07);
            ptr += sizeof(unsigned int);
            i++;
        } else if (format[i] == '%' && format[i + 1] == 'x') {
            int num = *(int*)ptr;
            char buffer[20];
            snprintf(buffer, sizeof(buffer), "%x", num);
            print(buffer, 0x07);
            ptr += sizeof(int);
            i++;
        } else if (format[i] == '%' && format[i + 1] == 'c') {
            char c = *(char*)ptr;
            char str[2] = {c, '\0'};
            print(str, 0x07);
            ptr += sizeof(char);
            i++;
        } else {
            char c[2] = {format[i], '\0'};
            print(c, 0x07);
        }
    }
}
