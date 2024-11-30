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
#include <declarations/lang.h>

#define AUROR_EXECUTABLE_HEADER "\0\0\0AEF-"
#define WINDOWS_EXECUTABLE_HEADER "MZ"
#define ELF_EXECUTABLE_HEADER "\x7fELF"

#define AUROR_EXECUTABLE_CODE 1
#define WINDOWS_EXECUTABLE_CODE 2
#define ELF_EXECUTABLE_CODE 3

int determine_executable_format(string content) {
    if (string_starts_with(content, AUROR_EXECUTABLE_HEADER)) {
        return AUROR_EXECUTABLE_CODE; // AurorOS Executable Format (AEF)
    }
    if (string_starts_with(content, WINDOWS_EXECUTABLE_HEADER)) {
        return WINDOWS_EXECUTABLE_CODE; // Portable (Windows/MS-DOS) Executable Format (PE)
    }
    if (string_starts_with(content, ELF_EXECUTABLE_HEADER)) {
        return ELF_EXECUTABLE_CODE; // Executable and Linkable Format (ELF)
    }
    return 0; // File content is not an executable
}

int start_aef_executable(string content) {
    return 0;
}

int start_executable(string content, string argv[]) {
    int executable_format = determine_executable_format(content);
    if (executable_format == 0) {
        return 1; // File is not an executable
    }
    if (executable_format == WINDOWS_EXECUTABLE_CODE || executable_format == ELF_EXECUTABLE_CODE) {
        return 2; // File is an executable, but has unsupported type
    }

    return start_aef_executable(content);
}