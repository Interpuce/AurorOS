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

struct Thread* thread_list = NULL;

void add_thread(struct Thread* new_thread) {
    if (thread_list == NULL) {
        thread_list = new_thread;
        return;
    }

    struct Thread* current = thread_list;
    while (current->next_thread != NULL) {
        current = current->next_thread;
    }
    current->next_thread = new_thread;
    new_thread->next_thread = NULL;
}

void remove_thread(struct Thread* thread_to_remove) {
    if (thread_list == NULL) return;

    if (thread_list == thread_to_remove) {
        thread_list = thread_list->next_thread;
        return;
    }

    struct Thread* current = thread_list;
    while (current->next_thread != NULL) {
        if (current->next_thread == thread_to_remove) {
            current->next_thread = thread_to_remove->next_thread;
            return;
        }
        current = current->next_thread;
    }
}