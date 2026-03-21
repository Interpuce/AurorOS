#pragma once

#include <types.h>

typedef enum SYSCALL_ERROR_CODE {
    SYSCALL_FD_NOT_FOUND = -1,
    SYSCALL_FILE_NOT_FOUND = -2,
    SYSCALL_PERMISSION_DENIED = -3,
    SYSCALL_RESERVED = -4,
    SYSCALL_ILLEGAL_OPERATION = -5,
    SYSCALL_ILLEGAL_MODE = -6,

    SYSCALL_OK = 0
} SYSCALL_ERROR_CODE;

typedef struct {
    uintptr_t rax;
    SYSCALL_ERROR_CODE errs;
    kbool set_rax_to_direct_return_value;
} SYSCALL_RESULT;

extern SYSCALL_RESULT handle_syscall(
    uintptr_t syscall_number, 
    uintptr_t rdi,
    uintptr_t rsi,
    uintptr_t rdx,
    uintptr_t user_id,
    uintptr_t process_id
);
