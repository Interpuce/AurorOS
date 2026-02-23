/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma GCC optimize ("O3")

extern "C" {
    #include <types.h>
    #include <msg.h>
    #include <string.h>
    #include <constants.h>
    #include <input.h>
    #include <screen.h>
    #include <fs/fs-emulated.h>
    #include <fs/filesystem.h>
    #include <string.h>
    #include <fs/permissions/check.h>
}

namespace ShellCommands {
    void cowsay(char message[1024]) {
        int message_length = strlen(message);

        print("  ", 0x07);
        for (int i = 0; i < message_length + 2; i++) {
            printchar('-', 0x07);
        }
        println("", 0x07);

        print(" < ", 0x07);
        print(message, 0x07);
        print(" > \n", 0x07);

        print("  ", 0x07);
        for (int i = 0; i < message_length + 2; i++) {
            printchar('-', 0x07);
        }
        println("", 0x07);

        print("   \\   ^__^\n", 0x07);
        print("    \\  (oo)\\_______\n", 0x07);
        print("       (__)\\       )\\/\n", 0x07);
        print("           ||---w||\n", 0x07);
        print("           ||    ||\n", 0x07);
    }

    void map() {
        print("\n", 0x07); 
        print(" 0x01", 0x01); print(" 0x09", 0x09);
        print(" 0x10", 0x10); print(" 0x90", 0x90);
        print("\n", 0x07); 
        print(" 0x02", 0x02); print(" 0x0A", 0x0A);
        print(" 0x20", 0x20); print(" 0xA0", 0xA0);
        print("\n", 0x07); 
        print(" 0x03", 0x03); print(" 0x0B", 0x0B);
        print(" 0x30", 0x30); print(" 0xB0", 0xB0);
        print("\n", 0x07); 
        print(" 0x04", 0x04); print(" 0x0C", 0x0C);
        print(" 0x40", 0x40); print(" 0xC0", 0xC0);
        print("\n", 0x07); 
        print(" 0x05", 0x05); print(" 0x0D", 0x0D);
        print(" 0x50", 0x50); print(" 0xD0", 0xD0);
        print("\n", 0x07); 
        print(" 0x06", 0x06); print(" 0x0E", 0x0E);
        print(" 0x60", 0x60); print(" 0xE0", 0xE0);
        print("\n", 0x07); 
        print(" 0x07", 0x07); print(" 0x0F", 0x0F);
        print(" 0x70", 0x70); print(" 0xF0", 0xF0);
        print("\n", 0x07); 
        print(" 0x08", 0x08); print(" 0x00", 0x00);
        print(" 0x80", 0x80); print(" 0x00", 0x00);
        println("\n", 0x07); 
        print(" ~!@#$%^&*()_+        `1234567890-= \n", 0x0F);
        print(" qwertyuiop[]\\        QWERTYUIOP{}| \n", 0x0F);
        print(" asdfghjkl;'          ASDFGHJKL:\" \n", 0x0F);
        print(" zxcvbnm,./           ZXCVBNM<>? \n", 0x0F);
        print("\n", 0x07); 
    }

    void eclair(const char *trt) {
        if (streql(trt, "1")) {
            print("\n", 0x07);
            for (uint8_t i = 0; i < 128; i++) {
                print(" ", 0x07);
                print("Hello, world! ", i);
            }
            print("\n", 0x07);
        } else if (streql(trt, "2")) {
            paintscreen(0x91);
        } else {
            print_error("Invalid treatment argument provided!");
        }
    }

    void help() {
        println("", 0x07);
        println(" ver - displays OS version info", 0x07);
        println(" print <arg1> - prints out <arg1>", 0x07);
        println(" map - prints out every VGA color and every printable character in aurorOS", 0x07);
        println(" cowsay <arg1> - displays ascii art of an cow saying <arg1>", 0x07);
        println(" tinypad - opens a text editor", 0x07);
        println(" eclair - a little development command", 0x07);
        println(" repo - get the AurorOS repo", 0x07);
        println(" reboot - reboots the computer (works only in QEMU)", 0x07);
        println(" shutdown - shuts down the computer (works only in QEMU)", 0x07);
        println(" ls - shows all files in the current directory", 0x07);
        println(" cat - reads a specified file", 0x07);
        println(" cd - changes the directory", 0x07);
        println(" chmod - changes permissions of a file", 0x07);
        println(" exit - exits the shell or shuts down the OS", 0x07);
        println("", 0x07);
    }

    void ls(fs_node* dir) {
        if (!dir) return;

        for (uint32_t i = 0; i < dir->child_count; i++) {
            if (!dir->children[i]) continue;

            print((const char*)dir->children[i]->name, 0x07);
            print(" ", 0x07);
        }

        println("", 0x07);
    }

    void cd(fs_node** current_dir, char* where, char* current_user) {
        if (!where || where[0] == 0) {
            print_error("Missing operand");
            return;
        }
    
        fs_node* target = fs_resolve(where, *current_dir);
        if (!target) {
            print_error("Directory not found");
            return;
        }
    
        if (target->type != EMULATED_FS_DIR) {
            print_info("Not a directory");
            return;
        }
    
        int is_owner = streql(target->owner, current_user);
    
        uint8_t perms = GET_PERMS(is_owner, is_owner, target->permissions);
        if (!(perms & 0x1)) { 
            print_error("Permission denied");
            return;
        }
    
        *current_dir = target;
    }

    uint16_t octal_to_uint16(const char* str) {
        uint16_t result = 0;
        for (int i = 0; str[i]; ++i) {
            char c = str[i];
            if (c < '0' || c > '7') break;
            result = (result << 3) | (c - '0');
        }   
        return result;
    }

    void chmod(fs_node* current_dir, char* current_user, char* perm_arg, char* target_path) {
        if (!perm_arg || !target_path) {
            print_error("Missing operand");
            return;
        }
    
        fs_node* target = fs_resolve(target_path, current_dir);
        if (!target) {
            print_error("File not found");
            return;
        }
    
        int is_owner = streql(current_user, target->owner);
        int is_root  = streql(current_user, "root");
    
        if (!is_owner && !is_root) {
            print_error("Permission denied");
            return;
        }
    
        uint16_t perms = target->permissions;

        if (perm_arg[0] >= '0' && perm_arg[0] <= '7') {
            uint16_t new_perms = ShellCommands::octal_to_uint16(perm_arg);
            target->permissions = new_perms;
            return;
        }

        for (int i = 0; perm_arg[i]; ++i) {
            if (perm_arg[i] == '+' || perm_arg[i] == '-') {
                char op = perm_arg[i];
                ++i;
                while (perm_arg[i]) {
                    uint8_t mask = 0;
                    switch (perm_arg[i]) {
                        case 'r': mask = 0x4; break;
                        case 'w': mask = 0x2; break;
                        case 'x': mask = 0x1; break;
                    }
                    if (op == '+') perms |= mask;
                    else perms &= ~mask;
                    ++i;
                }
                break;
            }
        }
    
        target->permissions = perms;
        print_info("Permissions updated");
    }

    void cat(fs_node* current_dir, char* where, char* current_user) {
        if (!where || where[0] == 0) {
            print_error("Missing operand");
            return;
        }

        fs_node* f = fs_resolve(where, current_dir);

        if (!f) {
            return print_error("File does not exist");
        }
        if (f->type != EMULATED_FS_FILE) {
            return print_info("Is a directory");
        }
        
        int is_owner = streql(f->owner, current_user);
    
        uint8_t perms = GET_PERMS(is_owner, is_owner, f->permissions);
        if (!(perms & 0x4)) { 
            print_error("Permission denied");
            return;
        }

        println_limit((char*)f->data, f->size, 0x07);
    }

    const char* fs_basename(const char* path) {
        int len = 0;
        while (path[len]) len++;
        if (len == 0) return path;

        int i = len - 1;
        while (i >= 0 && path[i] == '/') i--;
        if (i < 0) return path;
        while (i >= 0 && path[i] != '/') i--;

        return path + i + 1;
    }

    void mkdir(fs_node* current_dir, const char* where, char* owner) {
        if (!where || where[0] == 0) {
            print_error("Missing operand");
            return;
        }

        const char* name = fs_basename(where);
        if (!name || name[0] == 0) {
            print_error("Invalid name");
            return;
        }

        fs_node* existing = fs_resolve(where, current_dir);
        if (existing) {
            print_error("Cannot create directory: already exists");
            return;
        }

        fs_node* parent = current_dir;
        int last_slash = -1;

        for (int i = 0; where[i]; i++) {
            if (where[i] == '/') last_slash = i;
        }

        if (last_slash >= 0) {
            if (last_slash == 0) {
                parent = fs_resolve("/", current_dir);
            } else {
                char buffer[256];
                int len = last_slash;
                if (len > 255) len = 255;
                for (int i = 0; i < len; i++) buffer[i] = where[i];
                buffer[len] = 0;
                parent = fs_resolve(buffer, current_dir);
            }

            if (!parent) {
                print_error("Directory parent error: parent does not exist");
                return;
            }

            if (parent->type != EMULATED_FS_DIR) {
                print_error("Directory parent error: not a directory");
                return;
            }
        }

        fs_node* new_dir = fs_create_dir_node(name, parent, owner);
        fs_add_child(parent, new_dir);
    }

    void rm(fs_node* current_dir, const char* path) {
        if (!path || path[0] == 0) {
            print_error("Missing operand");
            return;
        }

        fs_node* node = fs_resolve(path, current_dir);
        if (!node) {
            print_error("No such file or directory");
            return;
        }

        if (node->type == EMULATED_FS_DIR) {
            kbool confirmation = read_yn("Do you want to delete a directory?\n (y/n) $ ", 0x07);
            if (confirmation == KFALSE) return print_info("Ommitting directory");
        }

        fs_node* parent = node->parent;
        if (!parent) {
            print_error("It is dangerous to operate recursively on '/'");
            print_error("If possible, please reformat the drive instead of using this command.");
            return;
        }

        fs_remove_child(parent, node);
        fs_delete(node);
    }

    const char* pwd(fs_node* current_dir) {
        static char path[256];
        int pos = 255;
        path[pos] = '\0';

        fs_node* node = current_dir;

        while (node != NULL) {
            if (node->parent == NULL) {
                //path[--pos] = '/';
                break;
            }

            int len = 0;
            while (node->name[len] != '\0') len++;

            for (int i = len - 1; i >= 0; i--) {
                path[--pos] = node->name[i];
            }

            path[--pos] = '/';
            node = node->parent;
        }

        const char* returnable = &path[pos];

        if (streql(returnable, "")) {
            returnable = "/";
        }

        return returnable;
    }
}
