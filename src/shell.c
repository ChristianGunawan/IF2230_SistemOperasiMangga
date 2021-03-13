// TODO : Recheck kernel size

#include "kernel-header/config.h" // TODO : Split getCursorPos, should not including kernel.h
#include "std-header/boolean.h"
#include "std-header/std.h"

// TODO : Wrap printColoredString() ?

void shell() {
    char string_buffer[1024], directory_string[1024], tp;
    string_buffer[0] = '\0';
    // TODO : Raw keypress wrapper for SCANCODE_UP_ARROW and SCANCODE_DOWN_ARROW last commands
    // TODO : strcpy(), cleanup
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
        print(">\n");
        tp = getFullKey();
        if (tp == 97)
            print("key registered");

    }

}
