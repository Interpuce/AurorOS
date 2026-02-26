#pragma once

#include <types.h>

typedef enum SYSCALL_ERROR_CODE {
    SYSCALL_FD_NOT_FOUND = -1,
    SYSCALL_FILE_NOT_FOUND = -2,
    SYSCALL_PERMISSION_DENIED = -3,
    SYSCALL_RESERVED = -4,
    SYSCALL_ILLEGAL_OPERATION = -5,
    SYSCALL_UNKNOWN = -6
} SYSCALL_ERROR_CODE;

extern SYSCALL_ERROR_CODE handle_syscall(
    uint64_t syscall_number, 
    uint64_t rdi,
    uint64_t rsi,
    uint64_t rdx,
    uint64_t r10,
    uint64_t r8,
    uint64_t r9,
    uint64_t user_id,
    uint64_t process_id
);
