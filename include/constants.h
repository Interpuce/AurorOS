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

/**
 * AurorOS version constant. It can be used to represent the version of
 * current running AurorOS instance.
 */
#define AUROR_VERSION "1.0.0"

/**
 * AurorOS alpha/beta state. It can be used to represent the latest, but not
 * stable releases and generally beta versions of the OS.
 * 
 * Can have three values:
 *  - 0 - means that version is the latest stable version 
 *  - 1 - beta version, can contains bugs, but it is stable in most cases
 *  - 2 - alpha version which can contain serious issues and displays
 *        warning before kernel launches first terminal proccess
 */
#define AUROR_BETA_STATE 2

/**
 * AurorOS alpha/beta subversion. When AUROR_BETA_STAGE is set to 1 or 2
 * it displays the subversion after version like this: AurorOS 1.0.0 (alpha 1)
 * 
 * When AUROR_BETA_STAGE is set to 0 this constant is ignored.
 */
#define AUROR_SUBVERSION "1"