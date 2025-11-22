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
#include <screen.h>
#include <string.h>

extern void cowsay(char message[1024]);
extern void map();
extern void eclair(const char *trt);
extern void help();
extern void cat(emulated_fs_node* current_dir, string where);
extern void cd(emulated_fs_node** current_dir, string where);
extern void ls(emulated_fs_node* dir);
extern char* pwd(emulated_fs_node* current_dir) ;
