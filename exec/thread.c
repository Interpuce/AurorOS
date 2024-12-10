/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include <memory.h>

#define STACK_SIZE 4096

typedef struct Thread {
    uint32_t* stack;
    uint32_t id;
    struct Thread* next;
} Thread;

Thread* current_thread = NULL;
Thread* thread_list = NULL;

void create_thread(void (*entry_point)(void*), void* arg) {
    Thread* new_thread = (Thread*)malloc(sizeof(Thread));
    new_thread->stack = (uint32_t*)malloc(STACK_SIZE) + STACK_SIZE;

    new_thread->stack[-1] = (uint32_t)arg;
    new_thread->stack[-2] = (uint32_t)entry_point;
    new_thread->stack[-3] = 0x10;
    new_thread->stack[-4] = 0x202;
    new_thread->stack[-5] = 0;
    new_thread->stack[-6] = 0;
    new_thread->stack -= 10;

    new_thread->next = thread_list;
    thread_list = new_thread;
    if (current_thread == NULL) {
        current_thread = new_thread;
    }
}

void switch_thread() {
    if (!current_thread || !current_thread->next) return;
    Thread* prev_thread = current_thread;
    Thread* next_thread = current_thread->next;
    uint32_t* esp = next_thread->stack;
    uint32_t* ebp = next_thread->stack + 1;
    asm volatile (
        "movl %0, %%esp\n"
        "movl %1, %%ebp\n"
        :
        : "r"(esp), "r"(ebp)
    );
    current_thread = next_thread;
    asm volatile (
        "movl %0, %%esp\n"
        "movl %1, %%ebp\n"
        :
        : "r"(next_thread->stack), "r"(next_thread->stack + 1)
    );
}

void scheduler() {
    while (1) {
        if (current_thread && current_thread->next) {
            switch_thread();
        }
    }
}