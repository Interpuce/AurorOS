extern "C" {
    #include <input.h>
    #include <msg.h>
    #include <string.h>
    #include <types.h>
    #include <numbers.h>
}

extern "C" void calculator_main() {
    int64_t left;
    int64_t right;
    char    sign[2]; // leaving a space for a null-terminator
                     
    char left_buf[32];
    char right_buf[32];
    char result_buf[32];

    while (KTRUE) {
        print("Sign (+ | - | / | *): ", 0x07);
        read_str(sign, 2, KFALSE, 0x07);

        if (
            !streql(sign, "/") && 
            !streql(sign, "*") && 
            !streql(sign, "-") && 
            !streql(sign, "+")
        ) {
            print_error("Invalid sign");
            continue;
        }

        break;
    }

    while (KTRUE) {
        print("Left number: ", 0x07);
        read_str(left_buf, 32, KFALSE, 0x07);

        if (!is_valid_int(left_buf)) {
            print_error("Invalid number");
            continue;
        }

        left = str_to_int64(left_buf);
        break;
    }

    while (KTRUE) {
        print("Right number: ", 0x07);
        read_str(right_buf, 32, KFALSE, 0x07);

        if (!is_valid_int(right_buf)) {
            print_error("Invalid number");
            continue;
        }

        right = str_to_int64(right_buf);
        break;
    }

    int64_t result = 0;

    if (streql(sign, "+")) {
        result = left + right;
    }
    else if (streql(sign, "-")) {
        result = left - right;
    }
    else if (streql(sign, "*")) {
        result = left * right;
    }
    else if (streql(sign, "/")) {
       result = left / right;
    }

    print("Result: ", 0x07);

    if (result < 0) {
        print("-", 0x07);
        utoa((uint64_t)(-result), result_buf, 10);
    } else {
        utoa((uint64_t)result, result_buf, 10);
    }

    print(result_buf, 0x07);
    print("\n", 0x07);
}
