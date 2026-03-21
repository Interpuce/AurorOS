#include "fs/fs-emulated.h"
#include <types.h>
#include <exec/fd.h>
#include <exec/syscall.h>
#include <fs/filesystem.h>
#include <fs/permissions/check.h>

#define RETE(code) ((SYSCALL_RESULT) { 0, code, KFALSE })
#define RETD(data) ((SYSCALL_RESULT) { data, SYSCALL_OK, KTRUE })

SYSCALL_RESULT handle_syscall(
    uintptr_t syscall_number, 
    uintptr_t rdi,
    uintptr_t rsi,
    uintptr_t rdx,
    uintptr_t user_id,
    uintptr_t process_id
) {
    switch (syscall_number) {
        case 0: {
            file_descriptor* fd = get_fd_from_id(rdi);
            if (!fd) 
                return RETE(SYSCALL_FD_NOT_FOUND);
            
            if (fd->opened_by_user != user_id || fd->opened_by_process != process_id) {
                return RETE(SYSCALL_PERMISSION_DENIED);
            }

            if (fd->use_fs_node) {
                fs_node* node = get_fs_node_from_fd(fd);
                if (!node)
                    return RETE(SYSCALL_FILE_NOT_FOUND);

                kbool is_owner = node->owner == user_id;
                kbool in_group = is_owner; // todo

                if (!(GET_PERMS(is_owner, in_group, node->permissions) & 2)) {
                    return RETE(SYSCALL_PERMISSION_DENIED); 
                }

                if (node->type != EMULATED_FS_FILE) {
                    return RETE(SYSCALL_ILLEGAL_MODE);
                }

                return RETD(fs_read(node, (uint8_t*)rsi, rdx));
            } else {
                return RETD(fd->iofunc.read((char*)rsi, rdx)); 
            }

            return RETE(SYSCALL_OK);
        }
        
        // WRITE
        case 1: {
            file_descriptor* fd = get_fd_from_id(rdi);
            if (!fd) 
                return RETE(SYSCALL_FD_NOT_FOUND);
            
            if (fd->opened_by_user != user_id || fd->opened_by_process != process_id) {
                return RETE(SYSCALL_PERMISSION_DENIED);
            }

            if (fd->use_fs_node) {
                fs_node* node = get_fs_node_from_fd(fd);
                if (!node)
                    return RETE(SYSCALL_FILE_NOT_FOUND);

                kbool is_owner = node->owner == user_id;
                kbool in_group = is_owner;

                if (!(GET_PERMS(is_owner, in_group, node->permissions) & 2)) {
                    return RETE(SYSCALL_PERMISSION_DENIED); 
                }

                if (node->type != EMULATED_FS_FILE) {
                    return RETE(SYSCALL_ILLEGAL_MODE);
                }

                fs_write(node, (char*)rsi, rdx);
            } else {
                fd->iofunc.write((char*)rsi, rdx);
            }

            return RETE(SYSCALL_OK);
        }

        // OPEN
        case 2: {
            fs_node* node = fs_resolve((const char*)rdi, fs_root);
            if (!node)
                return RETE(SYSCALL_FILE_NOT_FOUND);

            file_descriptor* fd = create_new_fsnode_fd(
                node,
                user_id,
                process_id
            );

            if (!fd)
                return RETE(SYSCALL_FD_NOT_FOUND);

            return RETD(fd->id);
        }

        default:
            return RETE(SYSCALL_ILLEGAL_OPERATION);
    }
}
