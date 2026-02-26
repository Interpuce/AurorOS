#pragma once

#include "fs/filesystem.h"
#include <types.h>

typedef struct file_descriptor {
    // base
    uint64_t id;

    // permissions
    uint64_t opened_by_process;
    uint64_t opened_by_user;

    // actual file information
    kbool use_fs_node;
    union {
        fs_node* path;
        struct {
            kbool(*read)(char*, size_t);
            kbool(*write)(char*, size_t);
        } iofunc;
    };

    // pointer to other fd 
    struct file_descriptor* next;
} file_descriptor;

extern fs_node* get_fs_node_from_fd(file_descriptor* fd);
file_descriptor* create_new_fsnode_fd(fs_node* node, uint64_t user_id, uint64_t proc_id);
file_descriptor* create_new_iofunc_fd(kbool(*read)(char*, size_t), kbool(*write)(char*, size_t), uint64_t user_id, uint64_t proc_id);
extern file_descriptor* get_fd_from_id(uint64_t id);
