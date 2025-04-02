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

#define AEF_BEGIN "##AEF--"
#define AEF_ARCHITECTURE_X86 "386---"
#define AEF_ARCHITECTURE_X64 "amd64-"
#define AEF_ARCHITECTURE_NOTHING "------"

extern int get_current_thread();