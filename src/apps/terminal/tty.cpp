#pragma GCC optimize("O3")

extern "C" {
    #include <types.h>
    #include <string.h>
    #include <fs/filesystem.h>
    #include <input.h>
    #include <msg.h>
    #include <constants.h>
    #include <panic.h>
}

namespace Auth {

    typedef struct {
        uint64_t user_id;
        char username[32];
        char password[32];
        char groups[8][32];
        int group_count;
    } UserEntry;

    void parse_users(const char* buf, int len, UserEntry* out, int* out_count) {
        int i = 0;
        *out_count = 0;

        while (i < len && *out_count < 16) {
            UserEntry* ue = &out[*out_count];
            ue->username[0] = 0;
            ue->password[0] = 0;
            ue->group_count = 0;
            ue->user_id = 0;

            int field = 0; // 0=user_id, 1=username, 2=password, 3=groups, 4=nothing
            int char_i = 0;
            int group_i = 0;
            int group_char_i = 0;

            while (i < len && buf[i] != '\n') {
                char c = buf[i];

                if (c == ':') {
                    if (field == 1) ue->username[char_i] = 0;
                    else if (field == 2) ue->password[char_i] = 0;
                    else if (field == 3 && group_char_i > 0 && group_i < 8) {
                        ue->groups[group_i][group_char_i] = 0;
                        ue->group_count++;
                        group_i++;
                        group_char_i = 0;
                    }
                    char_i = 0;
                    field++;
                }
                else if (field == 3) {
                    if (c == ',' && group_i < 8) {
                        ue->groups[group_i][group_char_i] = 0;
                        ue->group_count++;
                        group_i++;
                        group_char_i = 0;
                    } else if (group_char_i < 31) {
                        ue->groups[group_i][group_char_i++] = c;
                    }
                }
                else if (field == 1 || field == 2) {
                    if (char_i < 31) {
                        if (field == 1) ue->username[char_i++] = c;
                        else ue->password[char_i++] = c;
                    }
                }
                else if (field == 0) {
                    ue->user_id = 0;
                }

                i++;
            }

            if (field == 3 && group_char_i > 0 && group_i < 8) {
                ue->groups[group_i][group_char_i] = 0;
                ue->group_count++;
            }

            (*out_count)++;

            if (i < len && buf[i] == '\n') i++;
        }
    }

    UserEntry* find_user(const char* name, UserEntry* users, int count) {
        for (int i = 0; i < count; i++) {
            if (streql(users[i].username, name)) {
                return &users[i];
            }
        }
        return reinterpret_cast<UserEntry*>NULL;
    }
}

extern "C" int shell_main(uint16_t theme, char* current_user, uint64_t user_id);

extern "C" int terminal_main(uint16_t theme) {
    char buf[2048];
    Auth::UserEntry users[16];
    int users_count = 0;

reauth:
    static kbool welcome_msg_displayed = KFALSE;

    fs_node* node = fs_resolve("/etc/users", fs_root);
    if (!node) {
        kernelpanic("USERS_MISSING", "Cannot read the users file (/etc/users)\n or it is invalid. Login aborted.");
        goto reauth;
    }

    int len = fs_read(node, (uint8_t*)buf, sizeof(buf)-1);
    if (len < 0) {
        kernelpanic("USERS_MISSING", "Cannot read the users file (/etc/users)\n or it is invalid. Login aborted.");
        goto reauth;
    }
    buf[len] = 0;

    Auth::parse_users(buf, len, users, &users_count);

    if (welcome_msg_displayed == KFALSE) {
        println("", 0x07);
        if (AUROR_BETA_STATE == 1) {
            print_warn("You are using early build of AurorOS!");
        } else if (AUROR_BETA_STATE == 2) {
            print_warn("You are using public beta build of AurorOS!");
        } else if (AUROR_BETA_STATE == 3) {
            print_warn("You are using release candidate build of AurorOS!");
        } else {
            print_ok("You're running a stable version of AurorOS!");
        }
        println("", 0x07);
        println("Welcome to AurorOS!", 0x0a);
        if (Auth::find_user("liveuser", users, users_count) && AUROR_LIVEUSER_AUTOLOGIN) {
            println("Automatic login is enabled for the liveuser account.", 0x07);
        }
        println("", 0x07);
        welcome_msg_displayed = KTRUE;
    }

    static char username[32] = {0};
    print("AurorOS login: ", 0x07);
    read_str(username, sizeof(username), KFALSE, 0x07);

    if (username[0] == 0) {
        println("", 0x07);
        print_error("An username has to be provided.");
        println("", 0x07);
        goto reauth;
    }

    Auth::UserEntry* user = Auth::find_user(username, users, users_count);

    static char passwd[32] = {0};
    print(username, 0x07);
    print("@", 0x07);
    print(PC_NAME, 0x07);
    print(" password: ", 0x07);

    if (user && user != NULL && streql(username, "liveuser") && AUROR_LIVEUSER_AUTOLOGIN == KTRUE) {
        println("(automatic login)", 0x07);
    } else {
        read_str(passwd, sizeof(passwd), KTRUE, 0x07);
        if (!user || user == NULL || !streql(passwd, user->password)) {
            println("", 0x07);
            print_error("Login incorrect");
            println("", 0x07);
            goto reauth;
        }
    }

    println("", 0x07);

    shell_main(theme, user->username, user->user_id);

    println("", 0x07);

    goto reauth;

    return -1;
}
