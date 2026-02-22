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

char* replace_aef_arch(char* what) {
    if (streql("x86", what)) {
        return AEF_ARCHITECTURE_X86;
    }
    if (streql("x64", what)) {
        return AEF_ARCHITECTURE_X64;
    }
    return "------";
}

int check_aef_arch(char* content) {
    char* full_arch_beginning = AEF_MAGIC_NUMBER;
    strcpy(full_arch_beginning, replace_aef_arch(PC_ARCH));

    char* unsupported_arch = AEF_MAGIC_NUMBER;
    strcpy(unsupported_arch, "------");

    if (streql(full_arch_beginning, unsupported_arch)) {
        return CODE_EXEC_INVALID_ARCH;
    }

    if (starts_with(content, full_arch_beginning)) {
        return CODE_EXEC_ARCH_OK;
    } else {
        if (starts_with(content, AEF_MAGIC_NUMBER)) {
            return CODE_EXEC_UNSUPPORTED_ARCH_ON_THIS_DEVICE;
        } else {
            return CODE_EXEC_NOT_AN_EXECUTABLE;
        }
    }
}

int start_aef_binary(char* content) {
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
        kernelpanic("EXEC_ARCH_CHECK_ERROR", NULL);
        return 673;
    }
    return 0;
}