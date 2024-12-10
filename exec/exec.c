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

    // For now this does nothing, but I think we need
    //  to return any value.
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