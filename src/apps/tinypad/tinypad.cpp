#include "fs/fs-emulated.h"
extern "C" {
    #include <string.h>
    #include <screen.h>
    #include <msg.h>
    #include <types.h>
    #include <memory.h>
    #include <input.h>
    #include <fs/filesystem.h>
}

#define LINE_LENGTH 80
#define BUFFER_INCREMENT 150

fs_node* resolve_folder(const char* path) {
    fs_node* current = fs_root;
    char path_copy[256];
    strncpy(path_copy, path, sizeof(path_copy));
    path_copy[sizeof(path_copy) - 1] = '\0';

    char* ptr = path_copy;
    char* token;

    while ((token = strtok(&ptr, '/')) != NULL) {
        fs_node* child = fs_resolve(token, current);
        if (!child) {
            return reinterpret_cast<fs_node*>NULL;
        }
        current = child;
    }

    return current;
}

int tinypad_main(uint8_t color, uint8_t barcolor) {
    clearscreen();
    paintline(0, barcolor);
    printct("Tinypad", barcolor);
    println("\n\n `.exit` to discard and exit; `.save` to save and exit\n", 0x0F);

    size_t buffer_capacity = 1762;
    size_t buffer_len = 0;
    char* buffer = (char*)malloc(buffer_capacity);
    if (!buffer) {
        print_error("Memory allocation failed");
        return -1;
    }

    while (1) {
        char line[LINE_LENGTH + 1]; 
        read_str(line, sizeof(line), 0, color);

        if (starts_with(line, ".")) {
            if (streql(line, ".exit")) {
                break;
            } else if (streql(line, ".save")) {
                while (KTRUE) {
                    char fullpath[256];
                    print("File to save: ", 0x07);
                    read_str(fullpath, sizeof(fullpath), KFALSE, 0x07);

                    char* last_slash = strrchr(fullpath, '/');
                    char* filename;
                    fs_node* folder_node;

                    if (last_slash) {
                        *last_slash = '\0';
                        filename = last_slash + 1;
                        folder_node = resolve_folder(fullpath);
                    } else {
                        filename = fullpath;
                        folder_node = fs_root;
                    }

                    if (!folder_node) {
                        print_error("Folder does not exist");
                        continue;
                    }

                    fs_node* file_node = fs_resolve(filename, folder_node);
                    if (!file_node) {
                        file_node = fs_create_file_node(filename, folder_node, 1001);
                        fs_add_child(folder_node, file_node);
                    }

                    if (file_node->type == EMULATED_FS_DIR) {
                        print_info("Is a directory.");
                        continue;
                    }

                    fs_write(file_node, buffer, buffer_len);
                    print("File saved successfully.\n", 0x0A);
                    break;
                }
                break;
            } else {
                print_warn("This is not an editor command.");
                continue;
            }
        }

        size_t line_len = strlen(line);
        if (buffer_len + line_len + 1 > buffer_capacity) {
            char* new_buffer = (char*)malloc(buffer_capacity + BUFFER_INCREMENT);
            if (!new_buffer) {
                print_error("Out of memory!");
                break;
            }
            memcpy(new_buffer, buffer, buffer_len);
            free(buffer);
            buffer = new_buffer;
            buffer_capacity += BUFFER_INCREMENT;
        }

        memcpy(buffer + buffer_len, line, line_len);
        buffer_len += line_len;
        buffer[buffer_len++] = '\n';
    }

    clearscreen();
    free(buffer);
    return 0;
}
