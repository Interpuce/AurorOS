#include "msg.h"
#include <exec/fd.h>
#include <fs/filesystem.h>
#include <exec/syscall.h>
#include <types.h>
#include <fs/permissions/check.h>

SYSCALL_ERROR_CODE handle_syscall(
    uint64_t syscall_number, 
    uint64_t rdi,
    uint64_t rsi,
    uint64_t rdx,
    uint64_t r10,
    uint64_t r8,
    uint64_t r9,
    uint64_t user_id,
    uint64_t process_id
) {
    switch (syscall_number) {
        // SYSCALL 0 - READ
        //
        // registers:
        /// rdi - file descriptor
        /// rsi - pointer to a buffer in the
        ///       userspace
        /// rdx - maximal number of bytes
        ///       to be read
        case 0:
            return -1;
        
        // SYSCALL 1 - WRITE
        // 
        // registers:
        /// rdi - file descriptor
        /// rsi - pointer to a buffer in the
        ///       userspace
        /// rdx - number of bytes to be
        ///       written
        case 1: {
            if (rdi < 3) {
                switch (rdi) {
                    case 0:
                        return SYSCALL_ILLEGAL_OPERATION; // wtf?
                    case 1:
                        print((const char*)rsi, 0x07);
                        break;
                    case 2:
                        print((const char*)rsi, 0x07);
                        break;
                }

                return SYSCALL_RESERVED;
            }

            file_descriptor* fd = get_fd_from_id(rdi);
            if (fd == NULL) 
                return SYSCALL_FD_NOT_FOUND;

            fs_node* node = get_fs_node_from_fd(fd);
            if (node == NULL)
                return SYSCALL_FILE_NOT_FOUND;

            kbool is_owner = node->owner == user_id;
            kbool in_group = is_owner; // todo
           
            if (fd->opened_by_user != user_id) {
                return SYSCALL_PERMISSION_DENIED;
            }

            if (!(GET_PERMS(is_owner, in_group, node->permissions) & 2)) {
                return SYSCALL_PERMISSION_DENIED; 
            }

            fs_write(node, (char*)rsi, rdx);
        }

        // SYSCALL 2 - OPEN
        //
        // registers:
        /// rdi - pathname
        /// rsi - flags
        /// rdx - mode
        // 
        // notes: the kernel resolves the pathname given
        //        by a process
        case 2: {
            file_descriptor* fd = create_new_fd(
                fs_resolve((const char*)rdi, fs_root),
                user_id,
                process_id
            );

            if (fd == NULL) 
                return SYSCALL_FD_NOT_FOUND;

            return fd->id;
        }

        default:
            // who cares about error handling
            // jk, just to do or smth
            return -1;
    }
}
