#include "kernel-header/config.h"
#include "std-header/std_stringio.h"
#include "shell-header/shell_common.h"
#include "basic-header/std_opr.h"
#include "std-header/boolean.h"

void cp(char *dirtable, char current_dir_index, char flags, char *target, char *linkname);

int main() {
    char directory_table[FILES_SECTOR_SIZE*SECTOR_SIZE];
    char shell_cache[SECTOR_SIZE];
    char arg_vector[ARGC_MAX][ARG_LENGTH];
    char argc = 0;
    int current_dir_index;

    clear(shell_cache, SECTOR_SIZE);
    getDirectoryTable(directory_table);
    getShellCache(shell_cache);

    clear(arg_vector, ARGC_MAX*ARG_LENGTH);
    memcpy(arg_vector[0], shell_cache+ARGV_OFFSET, ARG_LENGTH);
    memcpy(arg_vector[1], shell_cache+ARGV_2_OFFSET, ARG_LENGTH);
    memcpy(arg_vector[2], shell_cache+ARGV_3_OFFSET, ARG_LENGTH);
    argc = shell_cache[ARGC_OFFSET];
    current_dir_index = shell_cache[CURRENT_DIR_CACHE_OFFSET];

    // Argument count
    if (argc >= 3) {
        if (!strcmp("-r", arg_vector[0]))
            cp(directory_table, current_dir_index, 1, arg_vector[1], arg_vector[2]);
        else
            cp(directory_table, current_dir_index, 0, arg_vector[0], arg_vector[1]);
    }
    else
        print("Usage : cp [-r] <source> <destination>\n", BIOS_WHITE);

    setShellCache(shell_cache);
    shellReturn();
}

// TODO : Autocomplete & relative pathing
void cp(char *dirtable, char current_dir_index, char flags, char *target, char *linkname) {
    // Technically just "copy" of previous implementation of ln
    // char as string / char
    char filename_buffer[16];
    char source_directory_name[16];
    char copied_directory_name[16];
    // char as 1 byte integer
    char file_read[FILE_SIZE_MAXIMUM];
    char target_entry_byte = 0;
    char source_dir_idx;
    char copied_dir_idx;
    int returncode_src = 0;
    int returncode_cpy = 0;
    bool is_write_success = false, valid_filename = true;
    bool f_target_found = false, empty_entry_found = false;
    int i = 0, j = 0;
    int f_entry_idx = 0;
    int f_entry_sector_idx = 0;
    int last_slash_index;


    // Relative pathing
    clear(source_directory_name, 16);
    clear(copied_directory_name, 16);
    // Split target / source
    last_slash_index = getLastMatchedCharIdx(CHAR_SLASH, target);
    // FIXME : Extra, unsafe getlast
    if (last_slash_index != -1) {
        // Split argument to path and filename
        // Get path
        strcpybounded(source_directory_name, target, last_slash_index);
        source_dir_idx = directoryEvaluator(dirtable, source_directory_name, &returncode_src, current_dir_index);

        // Get filename
        strcpybounded(source_directory_name, target+last_slash_index+1, ARG_LENGTH-last_slash_index-1);
    }
    else {
        // Cut slash
        strcpybounded(source_directory_name, target, ARG_LENGTH);

        source_dir_idx = current_dir_index;
        returncode_src = 0;
    }

    // Split destination / copied
    last_slash_index = getLastMatchedCharIdx(CHAR_SLASH, linkname);
    // FIXME : Extra, unsafe getlast
    if (last_slash_index != -1) {
        // Split argument to path and filename
        // Get path
        strcpybounded(copied_directory_name, linkname, last_slash_index);
        copied_dir_idx = directoryEvaluator(dirtable, copied_directory_name, &returncode_cpy, current_dir_index);

        // Get filename
        strcpybounded(copied_directory_name, linkname+last_slash_index+1, ARG_LENGTH-last_slash_index-1);
    }
    else {
        // Cut slash
        strcpybounded(copied_directory_name, linkname, ARG_LENGTH);

        copied_dir_idx = current_dir_index;
        returncode_src = 0;
    }


    // Copying file if path evaluation success
    if (returncode_src == -1) {
        print("cp: ", BIOS_GRAY);
        print(target, BIOS_GRAY);
        print(": target not found\n", BIOS_GRAY);
    }
    else if (returncode_cpy == -1) {
        print("cp: ", BIOS_GRAY);
        print(copied_directory_name, BIOS_GRAY);
        print(": path not found\n", BIOS_GRAY);
    }
    else {
        clear(file_read, FILE_SIZE_MAXIMUM);
        read(file_read, source_directory_name, &returncode_src, source_dir_idx);
        if (returncode_src == 0) {
            if (flags == 0) {
                // Simple copy, refuse folder
                if (file_read[0] != NULL) {
                    // FIXME : Will ignore ln for now
                    write(file_read, copied_directory_name, &returncode_cpy, copied_dir_idx);
                }
                else {
                    print("cp: error: ", BIOS_WHITE);
                    print(target, BIOS_WHITE);
                    print(" is a folder\n", BIOS_WHITE);
                    returncode_cpy = -1;
                }
            }
            else {
                // Recursive
                // TODO : Add

                // read(file_read, linkname, &returncode, target_dir);



                // else {
                //     print("cp: recursive copy error\n", BIOS_GRAY);
                //     returncode = -1;
                // }
            }
        }
        else {
            print("cp: error: ", BIOS_WHITE);
            print(target, BIOS_WHITE);
            print(" not found\n", BIOS_WHITE);
            returncode_cpy = -1;
        }

        if (returncode_src == 0 && returncode_cpy == 0) {
            print(linkname, BIOS_GRAY);
            print(": copy created\n", BIOS_GRAY);
        }
        else
            print("cp: file writing error\n", BIOS_GRAY);
    }
}
