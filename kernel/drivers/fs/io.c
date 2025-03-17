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
#include <fs/fat32.h>
#include <fs/disk.h>
#include <fs/iostream.h>
#include <msg.h>
#include "multiapi.h"

FileReadResult disk_read_file(string path) {
    FileReadResult result = {0, "", false, false};
    if (!fs || fs->type == FS_UNKNOWN) return result;

    uint32_t max = 0xFFFFFFFF;
    switch(fs->type) {
        case FS_FAT32:
            result.bytes_read = fat32_read_file(&fs->fat32, path, (uint8_t*)result.content, max, fs->is_atapi);
            result.success = true;
            break;
        case FS_NTFS:
            print_error("Cannot read data from the unsupported filesystem!");
            break;
        case FS_EXT2:
            print_error("Cannot read data from the unsupported filesystem!");
            break;
    }
    return result;
}

bool disk_write_file(string path, string what) {
    if (!fs || fs->type == FS_UNKNOWN) return false;
    return false;
}