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
#include <filesystem.h>

namespace ShellCommands {
    extern void cowsay(char message[1024]);
    extern void map();
    extern void eclair(const char *trt);
    extern void help();
    extern void cat(fs_node* current_dir, string where);
    extern void cd(fs_node** current_dir, string where);
    extern void ls(fs_node* dir);
    extern const char* pwd(fs_node* current_dir);
    extern void mkdir(fs_node* current_dir, const char* where);
    extern void rm(fs_node* current_dir, const char* path);
    extern void DriverManager(char* trts[10]);
}