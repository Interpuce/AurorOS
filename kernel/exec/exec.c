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
#include <string.h>
#include "exec.h"
#include <constants.h>
#include <codes.h>
#include <msg.h>
#include <panic.h>
#include <memory.h>
#include <speaker.h>
#include <asm/power.h>
#include <input.h>

int current_thread = -1;

string replace_aef_arch(string what) {
    if (streql("x86", what)) {
        return AEF_ARCHITECTURE_X86;
    }
    if (streql("x64", what)) {
        return AEF_ARCHITECTURE_X64;
    }
    return "------";
}

int check_aef_arch(string content) {
    const string full_arch_beginning = AEF_BEGIN;
    strcpy(full_arch_beginning, replace_aef_arch(PC_ARCH));

    string unsupported_arch = AEF_BEGIN;
    strcpy(unsupported_arch, AEF_ARCHITECTURE_NOTHING);

    if (streql(full_arch_beginning, unsupported_arch)) {
        return CODE_EXEC_INVALID_ARCH;
    }

    if (starts_with(content, full_arch_beginning)) {
        return CODE_EXEC_ARCH_OK;
    } else {
        if (starts_with(content, AEF_BEGIN)) {
            return CODE_EXEC_UNSUPPORTED_ARCH_ON_THIS_DEVICE;
        } else {
            return CODE_EXEC_NOT_AN_EXECUTABLE;
        }
    }
}

int start_aef_binary(string content, int permission_level) {
    if (permission_level != PERMISSION_LEVEL_MAIN && permission_level != PERMISSION_LEVEL_MUSEABLER && permission_level != PERMISSION_LEVEL_NORMAL_USER) {
        print_error("Invalid permissions. Please contact us on GitHub issues.");
        return 675;
    }

    int is_arch_ok = check_aef_arch(content);

    if (is_arch_ok == CODE_EXEC_UNSUPPORTED_ARCH_ON_THIS_DEVICE) {
        print_error("This AEF binary is unsupported on this architecture.");
        return 670;
    }
    if (is_arch_ok == CODE_EXEC_INVALID_ARCH) {
        print_error("This AEF binary is written for architecture that AurorOS does not recognise.");
        return 671;
    }
    if (is_arch_ok == CODE_EXEC_NOT_AN_EXECUTABLE) {
        print_error("This AEF binary is not a valid AEF executable.");
        return 672;
    }
    if (is_arch_ok != CODE_EXEC_ARCH_OK) {
        kernelpanic("EXEC_ARCH_CHECK_ERROR");
        return 673;
    }

    size_t prefix_len = strlen(AEF_BEGIN) + strlen(AEF_ARCHITECTURE_NOTHING);
    char *new_content = content + prefix_len;

    void* binary_memory = malloc(strlen(new_content));
    if (binary_memory == NULL) {
        print_error("Failed to allocate memory for AEF binary.");
        return 674;
    }
    
    memcpy(binary_memory, new_content, strlen(content));

    void (*execute)() = (void (*)())binary_memory;
    execute();
}

int get_current_thread() {
    return current_thread;
}

void syscall_handler() {
    int syscall_id = 0;
    asm("movl %%eax, %0" : "=r" (syscall_id));
    switch (syscall_id) {
        case 4:
            char *str = NULL;
            asm("movl %%ecx, %0" : "=r" (str));
            uint8_t color = 7;
            asm("movl %%edx, %0" : "=r" (color));
            print(str, color);
            break;
        case 5:
            uint32_t frequency = 10000;
            uint32_t duration = 10;
            asm("movl %%ebx, %0" : "=r" (frequency));
            asm("movl %%ecx, %0" : "=r" (duration));
            speaker(frequency, duration);
            break;
        case 6:
            kernelpanic("REPORTED_CRASH");
            break;
        case 7:
            shutdown();
            break;
        case 8:
            reboot();
            break;
        case 9:
            clearscreen();
            break;
        case 10:
            char *buffer = NULL;
            int max_len = 0;
            asm("movl %%ebx, %0" : "=r" (buffer));
            asm("movl %%ecx, %0" : "=r" (max_len));
            if (buffer == NULL || max_len == 0) {
                print_warn("The reading system call executed by the application does not make sense");
                break;
            }
            read_str(buffer, max_len, 0, 0x07);
            break;
        case 11:
            char *str = NULL;
            asm("movl %%ebx, %0" : "=r" (str));
            uint8_t color = 7;
            asm("movl %%ecx, %0" : "=r" (color));
            printct(str, color);
            break;
        default:
            print_warn("Application tried to execute unimplemented system call");
            break;
    }
}