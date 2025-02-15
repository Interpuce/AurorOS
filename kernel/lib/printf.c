#include <msg.h>

void printf(const char* format, ...) {
    char* ptr = (char*)&format + sizeof(char*);

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && format[i + 1] == 's') {
            char* str = *(char**)ptr;
            print(str);
            ptr += sizeof(char*);
            i++;
        } else if (format[i] == '%' && format[i + 1] == 'd') {
            int num = *(int*)ptr;
            char buffer[20];
            snprintf(buffer, sizeof(buffer), "%d", num);
            print(buffer);
            ptr += sizeof(int);
            i++;
        } else if (format[i] == '%' && format[i + 1] == 'u') {
            unsigned int num = *(unsigned int*)ptr;
            char buffer[20];
            snprintf(buffer, sizeof(buffer), "%u", num);
            print(buffer);
            ptr += sizeof(unsigned int);
            i++;
        } else if (format[i] == '%' && format[i + 1] == 'x') {
            int num = *(int*)ptr;
            char buffer[20];
            snprintf(buffer, sizeof(buffer), "%x", num);
            print(buffer);
            ptr += sizeof(int);
            i++;
        } else if (format[i] == '%' && format[i + 1] == 'c') {
            char c = *(char*)ptr;
            char str[2] = {c, '\0'};
            print(str);
            ptr += sizeof(char);
            i++;
        } else {
            char c[2] = {format[i], '\0'};
            print(c);
        }
    }
}
