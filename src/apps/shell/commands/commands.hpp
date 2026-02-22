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
#include <fs/filesystem.h>

namespace ShellCommands {
    extern void cowsay(char message[1024]);
    extern void map();
    extern void eclair(const char *trt);
    extern void help();
    extern void cat(fs_node* current_dir, char* where, char* current_user);
    extern void cd(fs_node** current_dir, char* where, char* current_user);
    extern void ls(fs_node* dir);
    extern const char* pwd(fs_node* current_dir);
    extern void mkdir(fs_node* current_dir, const char* where, char* owner);
    extern void rm(fs_node* current_dir, const char* path);
    extern void chmod(fs_node* current_dir, char* current_user, char* perm_arg, char* target_path);
}
