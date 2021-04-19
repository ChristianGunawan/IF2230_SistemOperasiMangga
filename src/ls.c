#include "kernel-header/config.h"
#include "std-header/std_stringio.h"
#include "shell-header/shell_common.h"
#include "basic-header/std_opr.h"

void ls(char *dirtable, char target_dir);

int main() {
    char directory_table[FILES_SECTOR_SIZE*SECTOR_SIZE];
    char shell_cache[SECTOR_SIZE];
    clear(shell_cache, SECTOR_SIZE);
    getDirectoryTable(directory_table);

    getShellCache(shell_cache);
    ls(directory_table, shell_cache[LS_TARGET_DIR_CACHE_OFFSET]);
    shell_cache[LS_TARGET_DIR_CACHE_OFFSET] = CHAR_NULL;
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
