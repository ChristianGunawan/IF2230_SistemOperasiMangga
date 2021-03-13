// TODO : Recheck kernel size

#include "header/shell.h"
#include "header/kernel.h" // TODO : Split getCursorPos, should not including kernel.h
#include "header/std.h"

// TODO : Wrap printColoredString() ?

void shell() {
    char string_buffer[1024], directory_string[1024];
    string_buffer[0] = '\0';
    // TODO : Raw keypress wrapper for SCANCODE_UP_ARROW and SCANCODE_DOWN_ARROW last commands
    // TODO : strcpy(), cleanup
    directory_string[0] = '/';
    directory_string[1] = '\0';
    while (1) {
        printColoredString("mangga", BIOS_GREEN);
        printString(":");
        printColoredString(directory_string, BIOS_BLUE);
        printString("$ ");
        readString(string_buffer);
        printString("\nInput : <");
        printString(string_buffer);
        printString(">\n");

    }

}
