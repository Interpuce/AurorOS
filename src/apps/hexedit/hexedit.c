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
    return 0xFF; 
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
        char low = hexdump[i] ? hexdump[i++] : '0'; 

        uint8_t hi_val = hex_char_to_value(high);
        uint8_t lo_val = hex_char_to_value(low);

        if (hi_val == 0xFF || lo_val == 0xFF)
            hi_val = lo_val = 0; 

        out_bytes[byte_index++] = (hi_val << 4) | lo_val;
    }

    return byte_index;
}

static void print_hexdump(const uint8_t* input_buffer, char* output_buffer, size_t input_size, size_t output_size) {
    const char hex_chars[] = "0123456789ABCDEF";
    size_t out_index = 0;

    for (size_t i = 0; i < input_size; i++) {
        if (out_index + 3 > output_size) break;
        uint8_t byte = input_buffer[i];
        output_buffer[out_index++] = hex_chars[(byte >> 4) & 0xF];
        output_buffer[out_index++] = hex_chars[byte & 0xF];
        output_buffer[out_index++] = ' ';
    }

    if (out_index > 0 && out_index <= output_size) output_buffer[out_index - 1] = '\0';
    else if (out_index < output_size) output_buffer[out_index] = '\0';
}

static void hexedit_save(fs_node* file, const char* hexdump_modified) {
    uint8_t* parsed_bytes = malloc(file->size);
    if (!parsed_bytes) return;

    size_t bytes_written = parse_hexdump_to_bytes(hexdump_modified, parsed_bytes, file->size);

    fs_write(file, (char*)parsed_bytes, bytes_written);

    free(parsed_bytes);
}

void hexedit_main(uint64_t userid, const char* filename, fs_node*** cdir) {
    fs_node* file = fs_resolve(filename, **cdir);
    if (!file) return print_error("File not found.");

    if (file->type == EMULATED_FS_DIR) return print_info("Is a directory.");

    if (!(GET_PERMS(file->owner == userid, file->owner == userid, file->permissions) & 0x2))
        return print_error("No write permissions.");

    size_t hex_size = file->size * 3 + 1;
    char* hexdump = malloc(hex_size);
    char* hexdump_modified = malloc(hex_size);
    if (!hexdump || !hexdump_modified) {
        free(hexdump); free(hexdump_modified);
        return print_error("Memory allocation failed.");
    }

    print_hexdump((uint8_t*)file->data, hexdump, file->size, hex_size);

    read_str(hexdump_modified, hex_size, KFALSE, 0x08, hexdump);

    hexedit_save(file, hexdump_modified);

    free(hexdump);
    free(hexdump_modified);
    print_info("File saved successfully.");
}
