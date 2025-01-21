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

#define AUROR_NAME "AurorOS (Debug & Development Preview)"
#define AUROR_VERSION "1.0.1"
typedef enum {
    EARLY_BUILD,
    PUBLIC_BETA,
    STABLE_BUILD
} BuildPhase;
#define AUROR_CODE_PHASE PUBLIC_BETA
#define PC_NAME "auror"
#define PC_ARCH "x86"
