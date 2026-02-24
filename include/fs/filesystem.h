#pragma once

#include <types.h>
#include <fs/fs-emulated.h>

typedef emulated_fs_node fs_node;

fs_node* fs_create_dir_node(const char* name, fs_node* parent, uint64_t owner);
fs_node* fs_create_file_node(const char* name, fs_node* parent, uint64_t owner);
void fs_add_child(fs_node* dir, fs_node* child);

fs_node* fs_resolve(const char* path, fs_node* current);
void fs_write(fs_node* node, char* data, uint32_t size);
int fs_read(fs_node* node, uint8_t* out, uint32_t max);
void fs_delete(fs_node* node);
extern void fs_remove_child(fs_node* parent, fs_node* child);

void init_fs();

extern fs_node* fs_root;
