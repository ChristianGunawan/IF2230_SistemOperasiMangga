// 13519214 - Shell

#include "kernel-header/config.h" // Only for BIOS Color
#include "std-header/boolean.h"
#include "std-header/std.h"

void shell() {
    char string_buffer[1024], directory_string[1024], tp;
    string_buffer[0] = '\0';
    directory_string[0] = '/';
    directory_string[1] = '\0';
    while (true) {
        print("mangga", BIOS_GREEN);
        print(":");
        print(directory_string, BIOS_BLUE);
        print("$ ");
        gets(string_buffer);
        print("\nInput : <");
        print(string_buffer);
        print(">\n\n");
    }

}
