/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include "multitasking.h"
#include <memory.h>

struct Thread* thread_list = NULL;
uint32_t latest_thread_id = 1;

void add_thread(thread_t* new_thread) {
    if (new_thread == NULL) return;
    if (thread_list == NULL) {
        thread_list = new_thread;
        return;
    }

    thread_t* current = thread_list;
    while (current->next_thread != NULL) {
        current = current->next_thread;
    }
    current->next_thread = new_thread;
    new_thread->next_thread = NULL;
}

void remove_thread(thread_t* thread_to_remove) {
    if (thread_list == NULL) return;
    if (thread_to_remove == NULL) return;

    if (thread_list == thread_to_remove) {
        thread_list = thread_list->next_thread;
        return;
    }

    thread_t* current = thread_list;
    while (current->next_thread != NULL) {
        if (current->next_thread == thread_to_remove) {
            current->next_thread = thread_to_remove->next_thread;
            return;
        }
        current = current->next_thread;
    }
}

thread_t create_thread(void (*entry_point)(void*), ThreadPriority priority, uint32_t stack_size, bool system_critical) {
    thread_t* new_thread;

    latest_thread_id++;
    new_thread->thread_id = latest_thread_id;

    if (priority == THREAD_PRIORITY_SYSPROCESS && !system_critical) {
        new_thread->priority = THREAD_PRIORITY_LOW; // terminating the process wouldn't be a good idea, but because someone wants highest possible priorities I say no thanks
    } else {
        new_thread->priority = priority;
    }

    new_thread->stack_pointer = (uint32_t*)malloc(stack_size * sizeof(uint32_t));

    new_thread->within_breakpoint = false;

    new_thread->registers.eax = 0; // safe to set to 0
    new_thread->registers.ebx = 0; // safe to set to 0
    new_thread->registers.ecx = 0; // safe to set to 0
    new_thread->registers.edx = 0; // safe to set to 0
    new_thread->registers.esi = 0; // safe to set to 0
    new_thread->registers.edi = 0; // safe to set to 0
    new_thread->registers.ebp = 0; // not understanding very well
    new_thread->registers.esp = (uint32_t)(new_thread->stack_pointer + stack_size - 1); // address of stack beginning
    new_thread->registers.eip = (uint32_t)entry_point; // entry point
    new_thread->registers.eflags = 0x200; // allow only for interrupts, yep i didn't find a way to prevent changing this register to unlock the world of posibilities

    new_thread->next_thread = NULL;

    add_thread(new_thread);
}