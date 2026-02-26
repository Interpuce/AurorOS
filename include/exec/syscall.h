#pragma once

#include <types.h>

typedef enum SYSCALL_ERROR_CODE {
    SYSCALL_FD_NOT_FOUND = -1,
    SYSCALL_FILE_NOT_FOUND = -2,
    SYSCALL_PERMISSION_DENIED = -3,
    SYSCALL_RESERVED = -4,
    SYSCALL_ILLEGAL_OPERATION = -5
} SYSCALL_ERROR_CODE;

extern SYSCALL_ERROR_CODE handle_syscall(
    uint32_t syscall_number, 
    uint32_t rdi,
    uint32_t rsi,
    uint32_t rdx,
    uint32_t r10,
    uint32_t r8,
    uint32_t r9,
    uint32_t user_id,
    uint32_t process_id
);
