#include <string.h>
#include <types.h>
#include <fs/fs-emulated.h>
#include <fs/filesystem.h>

fs_node* fs_root;

void fs_write(fs_node* node, char* data, uint32_t size) {
    emulated_fs_write(node, data, size);
}

int fs_read(fs_node* node, uint8_t* out, uint32_t max) {
    return emulated_fs_read(node, out, max);
}

fs_node* fs_resolve(const char* path, fs_node* current) {
    return emulated_fs_resolve(path, current);
}

fs_node* fs_create_dir_node(const char* name, fs_node* parent, char* owner) {
    return emulated_fs_create_dir_node(name, parent, owner);
}

fs_node* fs_create_file_node(const char* name, fs_node* parent, char* owner) {
    return emulated_fs_create_file_node(name, parent, owner);
}

void fs_add_child(fs_node* dir, fs_node* child) {
    emulated_fs_add_child(dir, child);
}

void fs_remove_child(fs_node* parent, fs_node* child) {
    if (!parent || !child || parent->type != EMULATED_FS_DIR) return;

    int found = 0;
    for (uint32_t i = 0; i < parent->child_count; i++) {
        if (parent->children[i] == child) {
            found = 1;
        }
        if (found && i < parent->child_count - 1) {
            parent->children[i] = parent->children[i + 1];
        }
    }
    if (found) parent->child_count--;
}

void fs_delete(fs_node* node) {
    if (!node) return;

    if (node->type == EMULATED_FS_DIR) {
        while (node->child_count > 0) {
            fs_delete(node->children[0]);
        }
    }

    if (node->parent) {
        fs_remove_child(node->parent, node);
    }

    emulated_fs_delete(node);
}

extern void emulated_fs_init();

void init_fs() {
    emulated_fs_init();
    fs_root = emulated_fs_root;
    
    // bin folder
    fs_node* bin = fs_create_dir_node("bin", fs_root, "root");
    bin->permissions = 0755;
    fs_add_child(fs_root, bin);
    
    // home dirs
    fs_node* home = fs_create_dir_node("home", fs_root, "root");
    home->permissions = 0755;
    fs_add_child(fs_root, home);

    fs_node* user = fs_create_dir_node("liveuser", home, "liveuser");
    fs_add_child(home, user);

    fs_node* root = fs_create_dir_node("root", fs_root, "root");
    root->permissions = 0755;
    fs_add_child(fs_root, root);
    
    // etc folder
    fs_node* etc = fs_create_dir_node("etc", fs_root, "root");
    etc->permissions = 0755;
    fs_add_child(fs_root, etc); 

    // user files
    fs_node* notes = fs_create_file_node("README.txt", user, "liveuser");
    fs_add_child(user, notes);
    fs_write(notes, "Welcome in AurorOS!", 23);
    
    // system files
    fs_node* users = fs_create_file_node("users", etc, "root");
    fs_add_child(etc, users);
    char* users_content = "0:root:x::\n1001:liveuser:x:sudo,sudonopasswd:";
    fs_write(users, users_content, strlen(users_content));

    fs_node* groups = fs_create_file_node("groups", etc, "root");
    fs_add_child(etc, groups);
    fs_write(groups, "group {\n  name = sudo\n  desc = All users in this group will be able to use the sudo command.\n}\n\ngroup {\n  name = sudonopasswd\n  desc = All users in this group will be able to use the sudo command without the need to enter the user password.\n}", 243);
}
