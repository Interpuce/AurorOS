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

typedef enum {
    EMULATED_FS_FILE,
    EMULATED_FS_DIR
} emulated_fs_node_type;

typedef struct emulated_fs_node {
    char name[32];
    emulated_fs_node_type type;
    struct emulated_fs_node* parent;

    uint8_t* data;
    uint32_t size;

    struct emulated_fs_node* children[64];
    uint32_t child_count;
} emulated_fs_node;

extern emulated_fs_node* emulated_fs_resolve(const char* path, emulated_fs_node* current_dir);
extern emulated_fs_node* emulated_fs_root;
extern void init_virtual_fs();