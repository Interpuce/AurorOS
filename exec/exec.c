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
#include <string.h>
#include <constants.h>
#include <codes.h>
#include <threading/allowed.h>
#include <panic.h>

#include "exec.h"
#include "thread.h"

string determine_aef_executable_architecture(string content) {
    const string FULL_AUROR_X86_EXECUTABLE_HEADER = strcat(AUROR_EXECUTABLE_HEADER, AUROR_X86_EXECUTABLE_HEADER);
    const string FULL_AUROR_X64_EXECUTABLE_HEADER = strcat(AUROR_EXECUTABLE_HEADER, AUROR_X64_EXECUTABLE_HEADER);

    if (!starts_with(content, AUROR_EXECUTABLE_HEADER)) {
        return "none"; // cannot define architecture, because file is not target executable
    }

    if (starts_with(content, FULL_AUROR_X86_EXECUTABLE_HEADER)) {
        return "x86";
    }
    if (starts_with(content, FULL_AUROR_X64_EXECUTABLE_HEADER)) {
        return "x64";
    }

    return "invalid"; // cannot define architecture, because file architecture declaration is invalid
}

void start_executable_thread(string content) {
    string bytecode_str = strslice("", content, 14);

    uint8_t *bytecode = (uint8_t *)bytecode_str.data;
    size_t bytecode_size = bytecode_str.length;

    void *executable_memory = malloc(bytecode_size);
    if (executable_memory == NULL) {
        kernelpanic("MEMORY_MANAGMENT");
        return;
    }

    memcpy(executable_memory, bytecode, bytecode_size);

    void (*start_function)(threading_functions_allowed *) = (void (*)(threading_functions_allowed *))executable_memory;
    start_function(function_table);

    free(executable_memory);
}

int start_aef_executable(string content) {
    string arch = determine_aef_executable_architecture(content);
    if (streql(arch, "none")) {
        return CODE_EXEC_NOT_AN_EXECUTABLE;
    }
    if (streql(arch, "invalid")) {
        return CODE_EXEC_INVALID_ARCH;
    }
    if (!streql(arch, PC_ARCH)) {
        return CODE_EXEC_UNSUPPORTED_ARCH_ON_THIS_DEVICE;
    }

    create_thread(start_executable_thread, content);

    return 0;
}

// Macro to start the AurorOS Executable Format executable
int start_executable(string content) {
    return start_aef_executable(content);
}

void init_threads_system() {
    // The second argument is used in other things, like starting the executable
    //  and is not required here, so we use simply 1 as argument.
    create_thread(scheduler, 1);
}