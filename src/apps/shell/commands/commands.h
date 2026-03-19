#pragma once

#include <types.h>
#include <screen.h>
#include <fs/filesystem.h>

extern void        shc_cowsay(char message[1024]);
extern void        shc_map();
extern void        shc_eclair(const char *trt);
extern void        shc_help();
extern void        shc_cat(fs_node* current_dir, char* where, uint64_t current_user_id);
extern void        shc_cd(fs_node** current_dir, char* where, uint64_t current_user);
extern void        shc_ls(fs_node* dir);
extern const char* shc_pwd(fs_node* current_dir);
extern void        shc_mkdir(fs_node* current_dir, const char* where, uint64_t owner);
extern void        shc_rm(fs_node* current_dir, const char* path);
extern void        shc_chmod(fs_node* current_dir, uint64_t current_user_id, char* perm_arg, char* target_path);
