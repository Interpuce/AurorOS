#include <types.h>
#include <msg.h>
#include <string.h>
#include <constants.h>
#include <input.h>
#include <screen.h>
#include <fs/fs-emulated.h>
#include <fs/filesystem.h>
#include <fs/permissions/check.h>
#include <numbers.h>

void shc_cowsay(const char* message) {
    int len = strlen(message);
    print("  ", 0x07);
    for (int i = 0; i < len + 2; i++) printchar('-', 0x07);
    println("", 0x07);
    print(" < ", 0x07);
    print(message, 0x07);
    print(" >\n", 0x07);
    print("  ", 0x07);
    for (int i = 0; i < len + 2; i++) printchar('-', 0x07);
    println("", 0x07);
    print("   \\   ^__^\n    \\  (oo)\\_______\n       (__)\\       )\\/ \n           ||---w||\n           ||    ||\n", 0x07);
}

void shc_map() {
    for (int row = 0; row < 8; row++) {
        for (int col = 1; col <= 4; col++) {
            uint8_t color = (row << 4) | (col - 1);
            print(" 0x", 0x07);
            char buf[3];
            utoa(color, buf, 16);
            print(buf, color);
        }
        println("", 0x07);
    }
    print(" ~!@#$%^&*()_+        `1234567890-= \nqwertyuiop[]\\        QWERTYUIOP{}| \nasdfghjkl;'          ASDFGHJKL:\" \nzxcvbnm,./           ZXCVBNM<>? \n", 0x0F);
}

void shc_eclair(const char* option) {
    if (streql(option, "1")) {
        for (uint8_t i = 0; i < 128; i++) {
            print(" Hello, world! ", i);
        }
        println("", 0x07);
    } else if (streql(option, "2")) {
        paintscreen(0x91);
    } else {
        print_error("Invalid treatment argument provided!");
    }
}

void shc_help() {
    const char* commands[] = {
        " ver - displays OS version info",
        " print <arg1> - prints out <arg1>",
        " map - prints out every VGA color and every printable character in aurorOS",
        " cowsay <arg1> - displays ascii art of an cow saying <arg1>",
        " tinypad - opens a text editor",
        " eclair - a little development command",
        " repo - get the AurorOS repo",
        " reboot - reboots the computer (works only in QEMU)",
        " shutdown - shuts down the computer (works only in QEMU)",
        " ls - shows all files in the current directory",
        " cat - reads a specified file",
        " cd - changes the directory",
        " chmod - changes permissions of a file",
        " exit - exits the shell or shuts down the OS",
        " calc - opens a calculator"
    };
    println("", 0x07);
    for (int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
        println(commands[i], 0x07);
    }
    println("", 0x07);
}

void shc_ls(fs_node* dir) {
    if (!dir) return;
    for (uint32_t i = 0; i < dir->child_count; i++) {
        if (!dir->children[i]) continue;
        print((const char*)dir->children[i]->name, 0x07);
        print(" ", 0x07);
    }
    println("", 0x07);
}

void shc_cd(fs_node** current_dir, const char* target_path, uint64_t user_id) {
    if (!target_path || target_path[0] == 0) {
        print_error("Missing operand");
        return;
    }
    fs_node* target = fs_resolve(target_path, *current_dir);
    if (!target) {
        print_error("Directory not found");
        return;
    }
    if (target->type != EMULATED_FS_DIR) {
        print_info("Not a directory");
        return;
    }
    int is_owner = target->owner == user_id;
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
        if (str[i] < '0' || str[i] > '7') break;
        result = (result << 3) | (str[i] - '0');
    }
    return result;
}

void shc_chmod(fs_node* current_dir, uint64_t user_id, const char* perm_arg, const char* path) {
    if (!perm_arg || !path) {
        print_error("Missing operand");
        return;
    }
    fs_node* target = fs_resolve(path, current_dir);
    if (!target) {
        print_error("File not found");
        return;
    }
    int is_owner = target->owner == user_id;
    int is_root = user_id == 0;
    if (!is_owner && !is_root) {
        print_error("Permission denied");
        return;
    }
    uint16_t perms = target->permissions;
    if (perm_arg[0] >= '0' && perm_arg[0] <= '7') {
        target->permissions = octal_to_uint16(perm_arg);
    } else {
        char op = perm_arg[0];
        for (int i = 1; perm_arg[i]; i++) {
            uint8_t mask = 0;
            switch (perm_arg[i]) {
                case 'r': mask = 0x4; break;
                case 'w': mask = 0x2; break;
                case 'x': mask = 0x1; break;
            }
            if (op == '+') perms |= mask;
            else if (op == '-') perms &= ~mask;
        }
        target->permissions = perms;
    }
    print_info("Permissions updated");
}

void shc_cat(fs_node* current_dir, const char* path, uint64_t user_id) {
    if (!path || path[0] == 0) {
        print_error("Missing operand");
        return;
    }
    fs_node* file = fs_resolve(path, current_dir);
    if (!file) {
        print_error("File does not exist");
        return;
    }
    if (file->type != EMULATED_FS_FILE) {
        print_info("Is a directory");
        return;
    }
    int is_owner = file->owner == user_id;
    uint8_t perms = GET_PERMS(is_owner, is_owner, file->permissions);
    if (!(perms & 0x4)) {
        print_error("Permission denied");
        return;
    }
    println_limit((char*)file->data, file->size, 0x07);
}

const char* fs_basename(const char* path) {
    int len = strlen(path);
    if (len == 0) return path;
    int i = len - 1;
    while (i >= 0 && path[i] == '/') i--;
    if (i < 0) return path;
    while (i >= 0 && path[i] != '/') i--;
    return path + i + 1;
}

void shc_mkdir(fs_node* current_dir, const char* path, uint64_t owner) {
    if (!path || path[0] == 0) {
        print_error("Missing operand");
        return;
    }
    const char* name = fs_basename(path);
    if (!name || name[0] == 0) {
        print_error("Invalid name");
        return;
    }
    if (fs_resolve(path, current_dir)) {
        print_error("Directory already exists");
        return;
    }
    fs_node* parent = current_dir;
    int last_slash = -1;
    for (int i = 0; path[i]; i++) if (path[i] == '/') last_slash = i;
    if (last_slash >= 0) {
        char buffer[256];
        int len = last_slash < 255 ? last_slash : 255;
        strncpy(buffer, path, len);
        buffer[len] = 0;
        parent = fs_resolve(buffer, current_dir);
        if (!parent || parent->type != EMULATED_FS_DIR) {
            print_error("Invalid parent directory");
            return;
        }
    }
    fs_node* new_dir = fs_create_dir_node(name, parent, owner);
    fs_add_child(parent, new_dir);
}

void shc_rm(fs_node* current_dir, const char* path) {
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
        if (read_yn("Delete directory? (y/n) $ ", 0x07) == KFALSE) {
            print_info("Omitting directory");
            return;
        }
    }
    fs_node* parent = node->parent;
    if (!parent) {
        print_error("Cannot remove root directory '/'");
        return;
    }
    fs_remove_child(parent, node);
    fs_delete(node);
}

const char* shc_pwd(fs_node* current_dir) {
    static char path[256];
    int pos = 255;
    path[pos] = '\0';
    fs_node* node = current_dir;
    while (node != NULL) {
        if (node->parent == NULL) break;
        int len = strlen(node->name);
        for (int i = len - 1; i >= 0; i--) path[--pos] = node->name[i];
        path[--pos] = '/';
        node = node->parent;
    }
    const char* result = &path[pos];
    return streql(result, "") ? "/" : result;
}
