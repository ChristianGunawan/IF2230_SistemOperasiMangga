#include "kernel-header/config.h"
#include "std-header/std_stringio.h"
#include "shell-header/shell_common.h"
#include "basic-header/std_opr.h"

char cd(char *dirtable, char *dirstr, char current_dir);

int main() {
    char directory_table[FILES_SECTOR_SIZE*SECTOR_SIZE];
    char shell_cache[SECTOR_SIZE];
    char dirstr[BUFFER_SIZE];
    char new_index;
    int argc;

    clear(shell_cache, SECTOR_SIZE);
    getDirectoryTable(directory_table);
    getShellCache(shell_cache);

    clear(dirstr, BUFFER_SIZE);
    memcpy(dirstr, shell_cache+ARGV_OFFSET, ARG_LENGTH);
    argc = shell_cache[ARGC_OFFSET];

    if (argc == 2) {
        new_index = cd(directory_table, dirstr, shell_cache[CURRENT_DIR_CACHE_OFFSET]);
        shell_cache[CURRENT_DIR_CACHE_OFFSET] = new_index;
    }
    else if (argc == 1)
        shell_cache[CURRENT_DIR_CACHE_OFFSET] = ROOT_PARENT_FOLDER;
    else
        print("Usage : cd <path>\n", BIOS_WHITE);

    setShellCache(shell_cache);
    shellReturn();
}

char cd(char *dirtable, char *dirstr, char current_dir) {
    int returncode = 0;
    char new_dir_idx = directoryEvaluator(dirtable, dirstr, &returncode, current_dir);
    // If success return new dir index
    if (returncode == 0) {
        return new_dir_idx;
    }
    else if (returncode == 1) {
        // Else, entry is not folder
        print("cd: target type is a file\n", BIOS_WHITE);
        return current_dir;
    }
    else {
        // Else, return original dir
        print("cd: path not found\n", BIOS_WHITE);
        return current_dir;
    }
}
