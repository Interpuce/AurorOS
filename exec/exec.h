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
#include <string.h>

#define AUROR_EXECUTABLE_HEADER "AEF-"
#define AUROR_X86_EXECUTABLE_HEADER "ef86-32;"
#define AUROR_X64_EXECUTABLE_HEADER "ad64-64;"

int start_aef_executable(string content);
int start_executable(string content);