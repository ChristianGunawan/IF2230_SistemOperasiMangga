#include "kernel-header/config.h"
#include "std-header/std_stringio.h"
#include "shell-header/shell_common.h"
#include "basic-header/std_opr.h"

void ls(char *dirtable, char target_dir);

int main() {
    char directory_table[FILES_SECTOR_SIZE*SECTOR_SIZE];
    char shell_cache[SECTOR_SIZE];
    char temp;
    int returncode = -1, target_directory;
    char dirstr[BUFFER_SIZE];
    char argc = 0;

    clear(shell_cache, SECTOR_SIZE);
    getDirectoryTable(directory_table);
    getShellCache(shell_cache);

    clear(dirstr, BUFFER_SIZE);
    memcpy(dirstr, shell_cache+ARGV_OFFSET, ARG_LENGTH);
    argc = shell_cache[ARGC_OFFSET];

    if (argc == 1 || argc == 2) {
        if (argc == 2)
            target_directory = directoryEvaluator(directory_table, dirstr, &returncode, shell_cache[CURRENT_DIR_CACHE_OFFSET]);
        else {
            target_directory = shell_cache[CURRENT_DIR_CACHE_OFFSET];
            returncode = 0;
        }

        if (returncode == 0)
            ls(directory_table, target_directory);
        else if (returncode == 1)
            print("ls: target is file\n", BIOS_WHITE);
        else
            print("ls: path not found\n", BIOS_WHITE);
    }
    else
        print("Usage : ls [directory]\n", BIOS_WHITE);

    setShellCache(shell_cache);
    shellReturn();
}

void ls(char *dirtable, char target_dir) {
    int i = 0;
    // char as string / char
    char filename_buffer[16];
    // Use char as 1 byte integer
    char parent_byte_entry, entry_byte_entry;
    while (i < FILES_ENTRY_COUNT) {
        parent_byte_entry = dirtable[FILES_ENTRY_SIZE*i+PARENT_BYTE_OFFSET];
        entry_byte_entry = dirtable[FILES_ENTRY_SIZE*i+ENTRY_BYTE_OFFSET];
        if (parent_byte_entry == target_dir && entry_byte_entry != EMPTY_FILES_ENTRY) {
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
