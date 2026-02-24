#pragma once

#include <types.h>

extern int64_t handle_syscall(
    uint64_t syscall_number, 
    uint64_t rdi,
    uint64_t rsi,
    uint64_t rdx,
    uint64_t r10,
    uint64_t r8,
    uint64_t r9,
    uint64_t user_id
);
