#include "fs/filesystem.h"
#include "types.h"
#include <exec/fd.h>
#include <memory.h>

file_descriptor* first_fd = NULL;
uint64_t id_counter = 3;

fs_node* get_fs_node_from_fd(file_descriptor* fd) {
    return fd->path;
}

file_descriptor* get_fd_from_id(uint64_t id) {
    file_descriptor* current = first_fd;
    if (current == NULL)
        return NULL;

    while (KTRUE) {
        if (current->id == id)
            return current;

        current = current->next;

        if (current == NULL)
            return NULL;
    }
}

file_descriptor* create_new_fd(fs_node* node, uint64_t user_id, uint64_t proc_id) {
    file_descriptor* fd = (file_descriptor*)malloc(sizeof(file_descriptor));
    if (!fd)
        return NULL;

    fd->id = id_counter++;
    fd->path = node;        
    fd->next = NULL;
    fd->opened_by_user = user_id;
    fd->opened_by_process = proc_id;

    file_descriptor* current = first_fd;

    if (current != NULL) { 
        while (current->next != NULL) {
            current = current->next;
        }

        current->next = fd;
    } else {
        first_fd = fd;
    }

    return fd;
}
