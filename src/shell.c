// 13519214 - Shell

#include "kernel-header/config.h" // Only for BIOS Color
#include "std-header/boolean.h"
#include "std-header/std.h"

#define BUFFER_SIZE 1024

// TODO : Extra, Extra, actually splitting to separate "app"

void fillBuffer(char *buffer, int count, char filler) {
    int i = 0;
    while (i < count) {
        buffer[i] = filler;
        i++;
    }
}

void shellInput(char *string) {
    // FIXME : Extra, Literally gets()
    char c, scancode;
    int i = 0, j = 0, max_i = 0, savedCursorRow, savedCursorCol, rawKey;
    enableKeyboardCursor();
    savedCursorRow = getCursorPos(1);
    savedCursorCol = getCursorPos(0);

    do {
        rawKey = getFullKeyPress();
        c = rawKey & 0xFF;      // AL Value
        scancode = rawKey >> 8; // AH Value
        // Warning : Prioritizing ASCII before scancode
        switch (c) {
            case CHAR_INPUT_NEWLINE:
                break;
            case CHAR_BACKSPACE:
                // If i is not at starting input pos, decrement
                if (i > 0)
                    i--;

                // Shift copy from deleted index
                j = i;
                while (j < max_i) {
                    string[j] = string[j + 1];
                    j++;
                }

                // If buffer not empty, decrement size by 1
                if (max_i > 0)
                    max_i--;

                string[max_i] = CHAR_SPACE; // For deleting last char
                string[max_i+1] = CHAR_NULL;
                setCursorPos(savedCursorRow, savedCursorCol);
                printString(string);

                setCursorPos(savedCursorRow, savedCursorCol + i);
                break;
            default:
                // If char (AL) is not ASCII Control codes, check scancode (AH)
                switch (scancode) {
                    case SCANCODE_DOWN_ARROW:
                    case SCANCODE_UP_ARROW:
                        // Ignored
                        break;
                    case SCANCODE_LEFT_ARROW:
                        if (i > 0)
                            i--;
                        break;
                    case SCANCODE_RIGHT_ARROW:
                        if (i < max_i)
                            i++;
                        break;
                    default:
                        directCharPrint(c, BIOS_GRAY);
                        string[i] = c;
                        if (i == max_i)
                            max_i++;
                        i++;
                }
                setCursorPos(savedCursorRow, savedCursorCol + i);
        }
    } while (c != CHAR_INPUT_NEWLINE);
    string[max_i] = CHAR_NULL; // Terminating string
    disableKeyboardCursor();
    setCursorPos(savedCursorRow + 1, 0);
}

void directoryStringBuilder(char *string, char *dirtable, char current_dir) {
    // Use as string / char
    char filename_buffer[16];
    // Use as 1 bytes integer
    char current_parent = 0, parent[64];
    // parent will contain indexes in reversed order
    int i = 0, parent_length = 0;
    if (current_dir == ROOT_PARENT_FOLDER)
        string[0] = '/';
    else {
        clear(parent, 64);
        // Traversing folder until reaching root

        current_parent = dirtable[current_dir*0x10+PARENT_BYTE_OFFSET];
        while (current_parent != ROOT_PARENT_FOLDER) {
            parent[parent_length] = current_parent;
            parent_length++;
            current_parent = dirtable[current_parent*0x10+PARENT_BYTE_OFFSET];
        }

        i = parent_length - 1;
        while (i >= 0) {
            strapp(string, "/");
            clear(filename_buffer, 16);
            strcpybounded(filename_buffer, dirtable+(parent[i]*0x10)+PATHNAME_BYTE_OFFSET, 14);
            strapp(string, filename_buffer);

            i--;
        }
        strapp(string, "/");
    }
}

void shell() {
    char string_buffer[BUFFER_SIZE], directory_string[BUFFER_SIZE];
    char current_dir_index = ROOT_PARENT_FOLDER;
    char directory_table[2][SECTOR_SIZE];
    int tp;
    getDirectoryTable(directory_table);

    clear(string_buffer, BUFFER_SIZE);
    clear(directory_string, BUFFER_SIZE);
    while (true) {
        print("mangga", BIOS_GREEN);
        print(":");
        clear(directory_string, BUFFER_SIZE);
        directoryStringBuilder(directory_string, directory_table, current_dir_index);
        print(directory_string, BIOS_BLUE);
        print("$ ");
        shellInput(string_buffer);
        print("\nInput : <");
        print(string_buffer);
        print(">\n\n");
    }

    // TODO : Check cursor position, handle out of screen case
    // TODO : Extra, posibility of additional INT 10H in asm
}
