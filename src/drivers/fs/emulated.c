/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 *
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include <memory.h>
#include <msg.h>
#include <string.h>
#include <fs-emulated.h>

emulated_fs_node* emulated_fs_root;

emulated_fs_node* emulated_fs_create_dir_node(const char* name, emulated_fs_node* parent) {
    emulated_fs_node* node = malloc(sizeof(emulated_fs_node));
    memset(node, 0, sizeof(emulated_fs_node));
    strcpy(node->name, name);
    node->type = EMULATED_FS_DIR;
    node->parent = parent;
    return node;
}

emulated_fs_node* emulated_fs_create_file_node(const char* name, emulated_fs_node* parent) {
    emulated_fs_node* node = malloc(sizeof(emulated_fs_node));
    memset(node, 0, sizeof(emulated_fs_node));
    strcpy(node->name, name);
    node->type = EMULATED_FS_FILE;
    node->parent = parent;
    return node;
}

void emulated_fs_add_child(emulated_fs_node* dir, emulated_fs_node* child) {
    if (dir->type != EMULATED_FS_DIR) return;
    dir->children[dir->child_count++] = child;
    child->parent = dir;
}

emulated_fs_node* emulated_fs_find_in(emulated_fs_node* dir, const char* name) {
    if (dir->type != EMULATED_FS_DIR) return NULL;

    for (uint32_t i = 0; i < dir->child_count; i++) {
        if (streql(dir->children[i]->name, name))
            return dir->children[i];
    }
    return NULL;
}

emulated_fs_node* emulated_fs_resolve(const char* path, emulated_fs_node* current_dir) {
    emulated_fs_node* current;

    if (path[0] == '/') {
        current = emulated_fs_root;
        path++; 
    } else {
        current = current_dir;
    }

    char token[32];
    uint32_t pos = 0;

    for (uint32_t i = 0; ; i++) {
        char c = path[i];

        if (c == '/' || c == '\0') {
            token[pos] = '\0';

            if (pos > 0) {
                if (strcmp(token, ".") == 0) {
                }
                else if (strcmp(token, "..") == 0) {
                    if (current->parent != NULL)
                        current = current->parent;
                }
                else {
                    current = emulated_fs_find_in(current, token);
                    if (!current) return NULL;
                }
            }

            pos = 0;
            if (c == '\0') break;
            continue;
        }

        token[pos++] = c;
    }

    return current;
}

void emulated_fs_write(emulated_fs_node* file, string data, uint32_t size) {
    if (file->type != EMULATED_FS_FILE) return;

    file->data = malloc(size);
    memcpy(file->data, data, size);
    file->size = size;
}

void emulated_fs_read(emulated_fs_node* file, uint8_t* out, uint32_t max) {
    if (file->type != EMULATED_FS_FILE) return;

    uint32_t to_copy = (file->size < max ? file->size : max);
    memcpy(out, file->data, to_copy);
}

void emulated_fs_init() {
    emulated_fs_root = emulated_fs_create_dir_node("/", NULL);
}