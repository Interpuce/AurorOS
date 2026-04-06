#include "fs/permissions/check.h"
#include "input.h"
#include <msg.h>
#include <fs/fs-emulated.h>
#include <fs/filesystem.h>
#include <types.h>
#include <memory.h>

static uint8_t hex_char_to_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') c = c + 32;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0xD; // fallback lmao
}

static int is_hex(char c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

static size_t parse_hexdump_to_bytes(const char* hexdump, uint8_t* out_bytes, size_t max_bytes) {
    size_t byte_index = 0;
    size_t i = 0;

    while (hexdump[i] != '\0' && byte_index < max_bytes) {
        while (hexdump[i] && !is_hex(hexdump[i])) i++;
        if (!hexdump[i]) break;
        char high = hexdump[i++];

        while (hexdump[i] && !is_hex(hexdump[i])) i++;
        if (!hexdump[i]) {
            out_bytes[byte_index++] = 0xDD;
            break;
        }
        char low = hexdump[i++];

        uint8_t hi = hex_char_to_value(high);
        uint8_t lo = hex_char_to_value(low);

        if (hi == 0xD || lo == 0xD)
            out_bytes[byte_index++] = 0xDD;
        else
            out_bytes[byte_index++] = (hi << 4) | lo;
    }

    return byte_index;
}

static void print_hexdump(char* input_buffer, char* output_buffer, size_t input_size, size_t output_size) {
    const char hex_chars[] = "0123456789ABCDEF";
    size_t out_index = 0;

    for (size_t i = 0; i < input_size; i++) {
        if (out_index + 3 > output_size) {
            break;
        }

        uint8_t byte = (unsigned char)input_buffer[i];
        output_buffer[out_index++] = hex_chars[(byte >> 4) & 0xF]; 
        output_buffer[out_index++] = hex_chars[byte & 0xF];
        output_buffer[out_index++] = ' ';
    }

    if (out_index > 0 && out_index <= output_size) {
        output_buffer[out_index - 1] = '\0';
    } else if (out_index < output_size) {
        output_buffer[out_index] = '\0';
    }
}

static void hexedit_save(fs_node* file, const char* hexdump_modified) {
    uint8_t parsed_bytes[2500]; 
    size_t bytes_written = parse_hexdump_to_bytes(hexdump_modified, parsed_bytes, sizeof(parsed_bytes));
    fs_write(file, (char*)parsed_bytes, bytes_written);  
}

void hexedit_main(uint64_t userid, const char* filename, fs_node*** cdir) {
    fs_node* file = fs_resolve(filename, **cdir); 
    if (!file) {
        print_error("File not found.");
        return;
    }

    if (file->type == EMULATED_FS_DIR) {
        return print_info("Is a directory.");
    }

    if (!(GET_PERMS(file->owner == userid, file->owner == userid, file->permissions) & 0x2)) {
        return print_error("No write permissions.");
    }

    char hexdump[2500];
    char hexdump_modified[2500];

    print_hexdump(file->data, hexdump, file->size, sizeof(hexdump));

    read_str(hexdump_modified, sizeof(hexdump_modified), KFALSE, 0x08, hexdump);

    hexedit_save(file, hexdump_modified);
}
