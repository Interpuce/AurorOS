extern "C" {
    #include <string.h>
    #include <screen.h>
    #include <msg.h>
    #include <types.h>
    #include <memory.h>
    #include <input.h>
    #include <fs/filesystem.h>
}

int tinypad_main(uint8_t color, uint8_t barcolor) {
    clearscreen();
    paintline(0, barcolor);
    printct("Tinypad", barcolor);
    println("\n\n `.exit` to discard and exit; `.save` to save (experimental) and exit\n", 0x0F);
    char* buffer = (char*)malloc(1762);
    while (1) {
        read_str(buffer, 1762, 0, color);
        if (starts_with(buffer, ".")) {
            if (streql(buffer, ".exit")) {
                break;
            } else if (streql(buffer, ".save")) {
                while (KTRUE) {
                    char filename[256];
                
                    print("File to save: ", 0x07);
                    read_str(filename, sizeof(filename), KFALSE, 0x07);
                
                    fs_node* node = fs_resolve(filename, fs_root);
                    if (!node) {
                        print_error("Invalid filename");
                        continue;
                    }

                    fs_write(node, buffer, strlen(buffer));

                    break;
                }

                break;
            } else {
                print_warn("This is not an editor command.");
            }
        }
    }
    clearscreen();
    return 0;
}
