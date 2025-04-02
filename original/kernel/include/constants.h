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

#define AUROR_NAME "AurorOS"
#define AUROR_VERSION "Public Beta 3"
#define AUROR_ORIGIN_BRANCH "project-vernon" // Name of branch that latest code changes originated from
#define PC_NAME "auror"
#define PC_ARCH "x86"

typedef enum {
    PERMISSION_LEVEL_NORMAL_USER = 1,
    PERMISSION_LEVEL_MUSEABLER,
    PERMISSION_LEVEL_MAIN
} PermissionLevel;