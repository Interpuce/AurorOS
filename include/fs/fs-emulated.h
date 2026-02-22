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

    uint16_t permissions;

    string* data;
    uint32_t size;

    struct emulated_fs_node* children[64];
    uint32_t child_count;
} emulated_fs_node;

emulated_fs_node* emulated_fs_create_dir_node(const char* name, emulated_fs_node* parent);
emulated_fs_node* emulated_fs_create_file_node(const char* name, emulated_fs_node* parent);
void emulated_fs_add_child(emulated_fs_node* dir, emulated_fs_node* child);
emulated_fs_node* emulated_fs_find_in(emulated_fs_node* dir, const char* name);
emulated_fs_node* emulated_fs_resolve(const char* path, emulated_fs_node* current);

void emulated_fs_write(emulated_fs_node* file, string data, uint32_t size);
int emulated_fs_read(emulated_fs_node* file, uint8_t* out, uint32_t max);

extern void emulated_fs_delete(emulated_fs_node* node);

void emulated_fs_init();

extern emulated_fs_node* emulated_fs_root;
