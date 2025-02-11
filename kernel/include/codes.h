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

// Code throwed by `check_aef_arch()` if the provided file content
// is not an executable.
#define CODE_EXEC_NOT_AN_EXECUTABLE -1
// Code throwed by `check_aef_arch()` if the provided file content
// has invalid and unsupported architecture declaration.
#define CODE_EXEC_INVALID_ARCH -2
// Code throwed by `check_aef_arch()` if the provided file content
// has architecture declaration that is not supported on the current
// device processor.
#define CODE_EXEC_UNSUPPORTED_ARCH_ON_THIS_DEVICE -3
// Code throwed by `check_aef_arch()` if everything is okay.
#define CODE_EXEC_ARCH_OK 0