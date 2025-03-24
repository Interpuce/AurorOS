/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma once

#include <types.h>

typedef enum {
    THREAD_PRIORITY_SYSPROCESS,    // highest value for system-critical threads
    THREAD_PRIORITY_HIGH,          // high priority & focused window
    THREAD_PRIORITY_NORMAL,        // for normal threads
    THREAD_PRIORITY_LOW            // for threads that are basically useless, but needs to work
} ThreadPriority;

typedef struct {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t eip;
    uint32_t eflags;
} ThreadEmulatedRegisters;

struct Thread;
typedef struct Thread thread_t;

struct Thread {
    uint32_t thread_id;
    ThreadPriority priority;
    ThreadEmulatedRegisters registers;
    uint32_t* stack_pointer;
    bool within_breakpoint;
    thread_t* next_thread;
};