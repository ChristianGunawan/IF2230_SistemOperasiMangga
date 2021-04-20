#include "std-header/std_fileio.h"
#include "std-header/std_stringio.h"
#include "basic-header/std_opr.h"
#include "kernel-header/config.h"
#include "std-header/boolean.h"
#include "shell-header/shell_common.h"

void shell();

// Shell convention
// cache 0-0xF reserved for argv passing and shell state
// cache[0xF] used as current working directory
// cache 1-0xF will filled with evaluated argv
// cache 0x10-0x4F, 0x50-0x8F, 0x90-0xCF, 0xD0-0x10F, 0x110-0x14F
//      used as shell history

int main() {
    char cache_buffer[SECTOR_SIZE];
    int returncode;
    // Cache reading
    clear(cache_buffer, SECTOR_SIZE);
    getShellCache(cache_buffer);

    // Empty cache case
    if (!forcestrcmp(EMPTY_CACHE, cache_buffer)) {
        clear(cache_buffer, SECTOR_SIZE);
        cache_buffer[CURRENT_DIR_CACHE_OFFSET] = ROOT_PARENT_FOLDER;
        cache_buffer[CACHE_SIGNATURE_OFFSET] = CACHE_SIGNATURE;
    }

    // TODO : Previous cache case

    setShellCache(cache_buffer);
    shell(cache_buffer);
    while (1);
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

        // Adding lower parent
        i = parent_length - 1;
        while (i >= 0) {
            strapp(string, "/");
            clear(filename_buffer, 16);
            strcpybounded(filename_buffer, dirtable+(parent[i]*FILES_ENTRY_SIZE)+PATHNAME_BYTE_OFFSET, 14);
            strapp(string, filename_buffer);

            i--;
        }

        // Adding topmost parent
        strapp(string, "/");
        clear(filename_buffer, 16);
        strcpybounded(filename_buffer, dirtable+current_dir*FILES_ENTRY_SIZE+PATHNAME_BYTE_OFFSET, 14);
        strapp(string, filename_buffer);
    }
}

void shellInput(char *commands_history, char *dirtable, char current_dir) {
    // char as string
    char string[BUFFER_SIZE];
    char move_string_buffer_1[BUFFER_SIZE];
    char move_string_buffer_2[BUFFER_SIZE];
    char arg_vector[ARGC_MAX][ARG_LENGTH];
    char temp_eval[ARGC_MAX*ARG_LENGTH];
    char to_be_completed[ARGC_MAX*ARG_LENGTH];
    char filename_buffer[16];
    // char as 1 byte integer
    char c, scancode;
    int i = 0, j = 0, max_i = 0, rawKey, dbg = 0;
    int split_i, split_j, split_k;
    int selected_his_idx = 0, current_eval_idx = 0;
    int savedCursorRow = getKeyboardCursor(1);
    int savedCursorCol = getKeyboardCursor(0);
    bool is_modified = false, is_autocomplete_available = false;
    bool is_between_quote_mark = false, autocomplete_found = false;
    int argc = 0, returncode = 0, matched_idx = 0;
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
                        // Note : Currently autocomplete not available for inside quote
                        // Part 1: command identification
                        // TODO : Extra, need strsplit so badly :(
                        // TODO : Extra, Extra, sometimes print failed (?)
                        // Arguments splitting
                        // Temporary cut string
                        string[max_i] = CHAR_NULL;
                        argc = 0;
                        clear(arg_vector, ARGC_MAX*ARG_LENGTH);
                        is_between_quote_mark = false;
                        split_i = 0;
                        split_j = 0;
                        split_k = 0;

                        while (string[split_i] != CHAR_NULL) {
                            // TODO : Extra, Extra, mixing double and single quote
                            // If found space in commands and not within double quote mark, make new
                            if (string[split_i] == CHAR_SPACE && split_j < ARGC_MAX && !is_between_quote_mark) {
                                split_k = 0;
                                split_j++;
                            }
                            else if (string[split_i] == CHAR_DOUBLE_QUOTE) {
                                // Toggling is_between_quote_mark
                                is_between_quote_mark = !is_between_quote_mark;
                            }
                            else {
                                // Only copy if char is not double quote
                                // and space outside double quote
                                arg_vector[split_j][split_k] = string[split_i];
                                split_k++;
                            }

                            split_i++;
                        }
                        argc = split_j + 1; // Due split_j is between counting space between 2 args

                        is_autocomplete_available = false;
                        if (!strcmp("ls",arg_vector[0]) || !strcmp("cat",arg_vector[0]) || !strcmp("cd",arg_vector[0]))
                            is_autocomplete_available = true;

                        if (!is_between_quote_mark && is_autocomplete_available) {
                            // Part 2: current index evaluation
                            current_eval_idx = current_dir;
                            matched_idx = getLastMatchedCharIdx(CHAR_SLASH, arg_vector[1]);
                            // If argv[1] is only single name, use original dir
                            if (matched_idx != -1) {
                                clear(temp_eval,ARGC_MAX*ARG_LENGTH);
                                strcpybounded(temp_eval, arg_vector[1], matched_idx);
                                current_eval_idx = directoryEvaluator(dirtable, temp_eval, &returncode, current_dir);
                            }


                            // Part 3: command autocompletion
                            // "To be completed" command (ex. cat mnt/abc/pqr -> pqr)
                            clear(to_be_completed, ARGC_MAX*ARG_LENGTH);
                            strcpy(to_be_completed, arg_vector[1]+matched_idx+1);
                            // Searching from directory table
                            autocomplete_found = false;
                            split_i = 0;
                            while (split_i < FILES_ENTRY_COUNT && !autocomplete_found) {
                                clear(filename_buffer, 16);
                                strcpybounded(filename_buffer, dirtable+FILES_ENTRY_SIZE*split_i+PATHNAME_BYTE_OFFSET, 14);
                                if (current_eval_idx == dirtable[FILES_ENTRY_SIZE*split_i+PARENT_BYTE_OFFSET]) {
                                    // Partial string comparation
                                    split_j = 0;
                                    autocomplete_found = true;
                                    // Set autocomplete_found as found, if string comparation below failed
                                    //       cancel searching status to not found
                                    while (to_be_completed[split_j] != CHAR_NULL && autocomplete_found) {
                                        if (to_be_completed[split_j] != filename_buffer[split_j])
                                            autocomplete_found = false;
                                        split_j++;
                                    }
                                }
                                split_i++;
                            }

                            if (autocomplete_found) {
                                if (matched_idx != -1) {
                                    // If using relative pathing, then find last slash location and insert completion
                                    strcpy(string+getLastMatchedCharIdx(CHAR_SLASH, string)+1, filename_buffer);
                                }
                                else {
                                    // If not using relative pathing, use space as insertion location
                                    strcpy(string+getFirstMatchedCharIdx(CHAR_SPACE, string)+1, filename_buffer);
                                }
                            }

                            // Autocomplete printing
                            setKeyboardCursor(savedCursorRow, savedCursorCol);
                            print("                                                                ", BIOS_GRAY);
                            setKeyboardCursor(savedCursorRow, savedCursorCol);

                            // Change proper i and max_i values
                            i = strlen(string);
                            max_i = i;

                            print(string, BIOS_GRAY);
                        }
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

void shell(char *cache) {
    // char as string / char
    char commands_history[MAX_HISTORY][BUFFER_SIZE]; // "FIFO" data type for commands
    char directory_string[BUFFER_SIZE];
    char arg_vector[ARGC_MAX][ARG_LENGTH];
    char directory_table[2][SECTOR_SIZE];
    // char as 1 byte integer
    char io_buffer[SECTOR_SIZE];
    char current_dir_index = cache[CURRENT_DIR_CACHE_OFFSET];
    char is_between_quote_mark = false;
    int temp, returncode;
    int i = 0, j = 0, k = 0, argc = 0;

    getDirectoryTable(directory_table);

    clear(commands_history, BUFFER_SIZE*MAX_HISTORY);
    memcpy(commands_history, cache+HISTORY_CACHE_OFFSET, BUFFER_SIZE*MAX_HISTORY);

    while (true) {
        clear(arg_vector, ARGC_MAX*ARG_LENGTH);
        clear(directory_string, BUFFER_SIZE);
        print("mangga", BIOS_GREEN);
        print(":", BIOS_GRAY);
        directoryStringBuilder(directory_string, directory_table, current_dir_index);
        print(directory_string, BIOS_LIGHT_BLUE);
        print("$ ", BIOS_GRAY);
        shellInput(commands_history, directory_table, current_dir_index);
        memcpy(cache+HISTORY_CACHE_OFFSET, commands_history, BUFFER_SIZE*MAX_HISTORY);

        // Scroll up if cursor at lower screen
        while (getKeyboardCursor(1) > 20) {
            scrollScreen();
            setKeyboardCursor(getKeyboardCursor(1)-1, 0);
            showKeyboardCursor();
        }

        // Arguments splitting
        i = 0;
        j = 0;
        k = 0;
        while (commands_history[0][i] != CHAR_NULL) {
            // TODO : Extra, Extra, mixing double and single quote
            // If found space in commands and not within double quote mark, make new
            if (commands_history[0][i] == CHAR_SPACE && j < ARGC_MAX && !is_between_quote_mark) {
                k = 0;
                j++;
            }
            else if (commands_history[0][i] == CHAR_DOUBLE_QUOTE) {
                // Toggling is_between_quote_mark
                is_between_quote_mark = !is_between_quote_mark;
            }
            else {
                // Only copy if char is not double quote
                // and space outside double quote
                arg_vector[j][k] = commands_history[0][i];
                k++;
            }

            i++;
        }
        argc = j + 1; // Due j is between counting space between 2 args


        // Command evaluation
        if (!strcmp("ls", arg_vector[0]))  {
            // TODO : Extra, split argv evaluator to program itself
            if (argc == 1 || argc > 1) {
                if (argc > 1) {
                    temp = directoryEvaluator(directory_table, arg_vector[1], &returncode, current_dir_index);
                    cache[LS_TARGET_DIR_CACHE_OFFSET] = (char) temp;
                }
                else {
                    cache[LS_TARGET_DIR_CACHE_OFFSET] = current_dir_index;
                    returncode = 0;
                }

                setShellCache(cache);

                if (returncode == 0)
                    exec("ls", 0x3000, BIN_PARENT_FOLDER);
                else if (returncode == 1)
                    print("ls: target is file\n", BIOS_WHITE);
                else
                    print("ls: path not found\n", BIOS_WHITE);
            }
            else
                print("Usage : ls\n", BIOS_WHITE);
        }
        else if (!strcmp("cat", arg_vector[0])) {
            if (argc == 2)
                print("TBA", BIOS_RED);
                // cat(directory_table, arg_vector[1], current_dir_index);
            else
                print("Usage : cat <filename>\n", BIOS_WHITE);
        }
        else if (!strcmp("ln", arg_vector[0])) {
            if (argc >= 3) {
                print("TBA", BIOS_RED);
                // if (!strcmp("-s", arg_vector[1]))
                //     ln(directory_table, current_dir_index, 1, arg_vector[2], arg_vector[3]);
                // else
                //     ln(directory_table, current_dir_index, 0, arg_vector[1], arg_vector[2]);
            }
            else
                print("Usage : ln [-s] <target> <linkname>\n", BIOS_WHITE);
        }
        else if (!strcmp("cd", arg_vector[0])) {
            if (argc == 2) {
                setShellCache(cache);
                exec("cd", 0x3000, BIN_PARENT_FOLDER);
            }
            else
                print("Usage : cd <path>\n", BIOS_WHITE);
        }
        else if (!strcmp("mkdir", arg_vector[0])) {
            if (argc == 2) {
                print("TBA", BIOS_RED);
                // mkdir(arg_vector[1], current_dir_index);
                getDirectoryTable(directory_table);
            }
            else
                print("Usage : mkdir <name>\n", BIOS_WHITE);
        }
        else if (!strcmp("echo", arg_vector[0])) {
            // Because shell structure is simple, just handle echo here
            if (argc <= 2)
                print(arg_vector[1], BIOS_WHITE);
            else if (!strcmp(">", arg_vector[2])) { // Sad redirection
                clear(io_buffer, SECTOR_SIZE);
                strcpybounded(io_buffer, arg_vector[1], SECTOR_SIZE);
                write(io_buffer, arg_vector[3], &returncode, current_dir_index);
                if (returncode == -1) {
                    print("echo: ", BIOS_WHITE);
                    print(arg_vector[3], BIOS_WHITE);
                    print(" exist ", BIOS_WHITE);
                }
                else {
                    // If success writing to file, load new updated dirtable
                    getDirectoryTable(directory_table);
                }
            }
        }
        else if (!strcmp("", arg_vector[0])) {
            // WARNING : Multiple space in single block will count as multiple argument due to argsplit above
            // FIXME : Extra, ^ fix this argsplitter
            // Empty string -> doing nothing
        }
        else {
            print(arg_vector[0], BIOS_WHITE);
            print(": command not found\n", BIOS_WHITE);
        }
    }

}
