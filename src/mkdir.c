#include "kernel-header/config.h"
#include "std-header/std_stringio.h"
#include "shell-header/shell_common.h"
#include "basic-header/std_opr.h"

void mkdir(char *foldername, char current_dir_index);

int main() {
    char directory_table[FILES_SECTOR_SIZE*SECTOR_SIZE];
    char shell_cache[SECTOR_SIZE];
    char dirstr[BUFFER_SIZE];
    char argc = 0;

    clear(shell_cache, SECTOR_SIZE);
    getDirectoryTable(directory_table);
    getShellCache(shell_cache);

    clear(dirstr, BUFFER_SIZE);
    memcpy(dirstr, shell_cache+ARGV_OFFSET, ARG_LENGTH);
    argc = shell_cache[ARGC_OFFSET];

    // Argument count
    if (argc == 2) {
       mkdir(dirstr, shell_cache[CURRENT_DIR_CACHE_OFFSET]);
       setShellCache(shell_cache);
    }
    else
        print("Usage : mkdir <name>\n", BIOS_WHITE);
    shellReturn();
}

void mkdir(char *foldername, char current_dir_index) {
    int returncode;
    write(FOLDER, foldername, &returncode, current_dir_index);
    switch (returncode) {
        case 0:
            // Do nothing
            break;
        case -1:
            print("mkdir: ", BIOS_WHITE);
            print(foldername, BIOS_WHITE);
            print(" exist\n", BIOS_WHITE);
            break;
        default:
            print("Usage : mkdir <name>\n");
    }
}
