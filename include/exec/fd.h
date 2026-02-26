#pragma once

#include "fs/filesystem.h"
#include <types.h>

typedef struct file_descriptor {
    // base
    uint64_t id;

    // permissions
    uint64_t opened_by_process;
    uint64_t opened_by_user;

    // fs node
    fs_node* path;

    // pointer to other fd 
    struct file_descriptor* next;
} file_descriptor;

extern fs_node* get_fs_node_from_fd(file_descriptor* fd);
file_descriptor* create_new_fd(fs_node* node, uint64_t user_id, uint64_t proc_id);
extern file_descriptor* get_fd_from_id(uint64_t id);
