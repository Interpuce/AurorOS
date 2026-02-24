#pragma once

typedef struct registers {
    unsigned int ds;

    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;

    unsigned int int_no;
    unsigned int err_code;

    unsigned int eip, cs, eflags, useresp, ss;
} registers_t;
