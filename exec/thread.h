/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma once

#include <types.h>

#define STACK_SIZE 4096

typedef struct Thread {
    uint32_t* stack;
    uint32_t id;
    struct Thread* next;
} Thread;

void create_thread(void (*entry_point)(void*), void* arg);
void scheduler(int non_used_var);