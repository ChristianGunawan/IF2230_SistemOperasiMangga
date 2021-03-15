// 13519214 - Shell
// TODO : Extra, write to special sector dedicated for history
// TODO : Extra, Extra, special sector for configuration
// Note : Need interrupt() if linked without kernel, check other/interrupt.asm

#include "kernel-header/config.h" // Only for BIOS Color
#include "std-header/boolean.h"
#include "std-header/std.h"

#define BUFFER_SIZE 256
#define MAX_HISTORY 5

// TODO : Extra, Extra, actually splitting to separate "app"
void getDirectoryTable(char *buffer);
// WARNING : No bound checking
// Get all directory table, put in buffer







void getDirectoryTable(char *buffer) {
    // WARNING : Naive implementation
    interrupt(0x21, 0x0002, buffer, FILES_SECTOR, 0);
    interrupt(0x21, 0x0002, buffer + SECTOR_SIZE, FILES_SECTOR + 1, 0);
}

void fillBuffer(char *buffer, int count, char filler) {
    int i = 0;
    while (i < count) {
        buffer[i] = filler;
        i++;
    }
}

void directoryStringBuilder(char *string, char *dirtable, char current_dir) {
    // Use as string / char
    char filename_buffer[16];
    // Use as 1 bytes integer
    char current_parent = 0, parent[FILES_ENTRY_COUNT];
    // parent will contain indexes in reversed order
    int i = 0, parent_length = 0;
    if (current_dir == ROOT_PARENT_FOLDER)
        string[0] = '/';
    else {
        clear(parent, FILES_ENTRY_COUNT);
        // Traversing folder until reaching root
        current_parent = dirtable[current_dir*FILES_ENTRY_SIZE+PARENT_BYTE_OFFSET];
        while (current_parent != ROOT_PARENT_FOLDER) {
            parent[parent_length] = current_parent;
            parent_length++;
            current_parent = dirtable[current_parent*FILES_ENTRY_SIZE+PARENT_BYTE_OFFSET];
        }

        i = parent_length - 1;
        while (i >= 0) {
            strapp(string, "/");
            clear(filename_buffer, 16);
            strcpybounded(filename_buffer, dirtable+(parent[i]*FILES_ENTRY_SIZE)+PATHNAME_BYTE_OFFSET, 14);
            strapp(string, filename_buffer);

            i--;
        }
        strapp(string, "/");
    }
}

void shellInput(char *commands_history) {
    // char as string
    char string[BUFFER_SIZE];
    char move_string_buffer_1[BUFFER_SIZE];
    char move_string_buffer_2[BUFFER_SIZE];
    // char as 1 byte integer
    char c, scancode;
    int i = 0, j = 0, max_i = 0, rawKey, dbg = 0;
    int selected_his_idx = 0;
    int savedCursorRow = getKeyboardCursor(1);
    int savedCursorCol = getKeyboardCursor(0);
    bool is_modified = false;
    showKeyboardCursor();

    // Move history up
    strcpybounded(move_string_buffer_1, commands_history, BUFFER_SIZE - 1);
    while (i < MAX_HISTORY - 1) {
        strcpybounded(move_string_buffer_2, commands_history+BUFFER_SIZE*(i+1), BUFFER_SIZE - 1);
        strcpybounded(commands_history+BUFFER_SIZE*(i+1), move_string_buffer_1, BUFFER_SIZE - 1);
        strcpybounded(move_string_buffer_1, move_string_buffer_2, BUFFER_SIZE - 1);
        i++;
    }
    clear(commands_history, BUFFER_SIZE); // Delete first entry

    i = 0;
    selected_his_idx = 0;
    do {
        rawKey = getFullKey();
        c = rawKey & 0xFF;      // AL Value
        scancode = rawKey >> 8; // AH Value
        // WARNING : Prioritizing ASCII before scancode
        switch (c) {
            case CHAR_INPUT_NEWLINE:
                break;
            case CHAR_BACKSPACE:
                is_modified = true;
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
                setKeyboardCursor(savedCursorRow, savedCursorCol);
                print(string, BIOS_GRAY);

                setKeyboardCursor(savedCursorRow, savedCursorCol + i);
                break;
            default:
                // If char (AL) is not ASCII Control codes, check scancode (AH)
                switch (scancode) {
                    case SCANCODE_TAB:
                        // TODO : Auto complete here
                        break;
                    case SCANCODE_DOWN_ARROW:
                    case SCANCODE_UP_ARROW:
                        if (scancode == SCANCODE_DOWN_ARROW && selected_his_idx > 0)
                            selected_his_idx--;
                        else if (scancode == SCANCODE_UP_ARROW && selected_his_idx < MAX_HISTORY - 1)
                            selected_his_idx++;

                        setKeyboardCursor(savedCursorRow, savedCursorCol);
                        print("                                                                ", BIOS_GRAY);
                        setKeyboardCursor(savedCursorRow, savedCursorCol);
                        // Move current buffer to history first location, only if string is modified
                        if (is_modified) {
                            string[max_i] = CHAR_NULL;
                            strcpybounded(commands_history, string, BUFFER_SIZE - 1);
                        }

                        // Load command from history
                        strcpybounded(string, commands_history+(selected_his_idx*BUFFER_SIZE), BUFFER_SIZE - 1);
                        // Change proper i and max_i values
                        i = strlen(string);
                        max_i = i;

                        print(commands_history+(selected_his_idx*BUFFER_SIZE), BIOS_GRAY);
                        is_modified = false;
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
                        is_modified = true;
                        putchar(c);
                        string[i] = c;
                        if (i == max_i)
                            max_i++;
                        i++;
                }
                setKeyboardCursor(savedCursorRow, savedCursorCol + i);
        }
    } while (c != CHAR_INPUT_NEWLINE);
    string[max_i] = CHAR_NULL; // Terminating string
    hideKeyboardCursor();
    setKeyboardCursor(savedCursorRow + 1, 0); // TODO : Extra, for multi-line input, maybe can be adjusted

    strcpybounded(commands_history, string, BUFFER_SIZE - 1);
}


// TODO : Extra, Split file
void ls(char *dirtable, char current_dir) {
    int i = 0;
    // char as string / char
    char filename_buffer[16];
    // Use char as 1 byte integer
    char parent_byte_entry, entry_byte_entry;
    while (i < FILES_ENTRY_COUNT) {
        parent_byte_entry = dirtable[FILES_ENTRY_SIZE*i+PARENT_BYTE_OFFSET];
        entry_byte_entry = dirtable[FILES_ENTRY_SIZE*i+ENTRY_BYTE_OFFSET];
        if (parent_byte_entry == current_dir && entry_byte_entry != EMPTY_FILES_ENTRY) {
            clear(filename_buffer, 16);
            strcpybounded(filename_buffer, dirtable+FILES_ENTRY_SIZE*i+PATHNAME_BYTE_OFFSET, 14);
            if (isCharInString(CHAR_SPACE, filename_buffer)) {
                print("\"");
                print(filename_buffer, BIOS_LIGHT_GREEN);
                print("\"");
            }
            else
                print(filename_buffer, BIOS_LIGHT_GREEN);
            print(" ");
        }
        i++;
    }
    print("\n");
}

void cd(); // TODO : Add

void cat(char *filename, char current_dir) {
    char file_read[FILE_SIZE_MAXIMUM];
    int returncode = 0;
    clear(file_read, FILE_SIZE_MAXIMUM);
    readFile(file_read, filename, &returncode, current_dir);
    if (returncode == -1)
        print("cat: Error file not found\n");
    else
        print(file_read);
}

void ln();

// TODO : Other misc command (mkdir, rm, etc)

void shell() {
    char commands_history[MAX_HISTORY][BUFFER_SIZE]; // "FILO" data type for commands
    char directory_string[BUFFER_SIZE];
    char directory_table[2][SECTOR_SIZE];
    char current_dir_index = ROOT_PARENT_FOLDER;
    int i = 0;

    getDirectoryTable(directory_table);

    clear(commands_history, BUFFER_SIZE*MAX_HISTORY);
    clear(directory_string, BUFFER_SIZE);

    while (true) {
        print("mangga", BIOS_GREEN);
        print(":", BIOS_GRAY);
        clear(directory_string, BUFFER_SIZE);
        directoryStringBuilder(directory_string, directory_table, current_dir_index);
        print(directory_string, BIOS_BLUE);
        print("$ ", BIOS_GRAY);
        shellInput(commands_history, current_dir_index);
        // Scroll up if cursor at lower screen
        while (getCursorPos(1) > 20) {
            scrollScreen();
            setCursorPos(getCursorPos(1)-1, 0);
        }
        cat("nope", ROOT_PARENT_FOLDER);

    }

}
