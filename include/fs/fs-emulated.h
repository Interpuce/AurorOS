#pragma once

#include <types.h>

typedef enum {
    EMULATED_FS_FILE,
    EMULATED_FS_DIR
} emulated_fs_node_type;

typedef struct emulated_fs_node {
    // base
    char name[32];
    emulated_fs_node_type type;
    struct emulated_fs_node* parent;

    // permissions
    uint16_t permissions;
    uint64_t owner;

    // contents
    char* data;
    uint32_t size;

    // children
    struct emulated_fs_node* children[64];
    uint32_t child_count;
} emulated_fs_node;

emulated_fs_node* emulated_fs_create_dir_node(const char* name, emulated_fs_node* parent, uint64_t owner);
emulated_fs_node* emulated_fs_create_file_node(const char* name, emulated_fs_node* parent, uint64_t owner);
void emulated_fs_add_child(emulated_fs_node* dir, emulated_fs_node* child);
emulated_fs_node* emulated_fs_find_in(emulated_fs_node* dir, const char* name);
emulated_fs_node* emulated_fs_resolve(const char* path, emulated_fs_node* current);

void emulated_fs_write(emulated_fs_node* file, char* data, uint32_t size);
int emulated_fs_read(emulated_fs_node* file, uint8_t* out, uint32_t max);

extern void emulated_fs_delete(emulated_fs_node* node);

void emulated_fs_init();

extern emulated_fs_node* emulated_fs_root;
