#include "fs/filesystem.h"
#include "types.h"
#include <exec/fd.h>
#include <memory.h>

file_descriptor* first_fd = NULL;
uint64_t id_counter = 1;

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

file_descriptor* create_new_fsnode_fd(fs_node* node, uint64_t user_id, uint64_t proc_id) {
    file_descriptor* fd = (file_descriptor*)malloc(sizeof(file_descriptor));
    if (!fd)
        return NULL;

    fd->id = id_counter++;
    fd->path = node;        
    fd->next = NULL;
    fd->opened_by_user = user_id;
    fd->opened_by_process = proc_id;
    fd->use_fs_node = KTRUE;

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

file_descriptor* create_new_iofunc_fd(kbool(*read)(char*, size_t), kbool(*write)(char*, size_t), uint64_t user_id, uint64_t proc_id) {
    file_descriptor* fd = (file_descriptor*)malloc(sizeof(file_descriptor));
    if (!fd)
        return NULL;

    fd->id = id_counter++;
    fd->iofunc.read = read;
    fd->iofunc.write = write;
    fd->next = NULL;
    fd->opened_by_user = user_id;
    fd->opened_by_process = proc_id;
    fd->use_fs_node = KFALSE;

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
