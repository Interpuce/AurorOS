#include <syscall.h>
#include <types.h>

int64_t handle_syscall(
    uint64_t syscall_number, 
    uint64_t rdi,
    uint64_t rsi,
    uint64_t rdx,
    uint64_t r10,
    uint64_t r8,
    uint64_t r9,
    uint64_t user_id
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
        case 1:
            return -1;

        // SYSCALL 2 - OPEN
        //
        // registers:
        /// rdi - pathname
        /// rsi - flags
        /// rdx - mode
        // 
        // notes: the kernel resolves the pathname given
        //        by a process
        case 2:
            return -1;

        default:
            // who cares about error handling
            // jk, just to do or smth
            return -1;
    }
}
