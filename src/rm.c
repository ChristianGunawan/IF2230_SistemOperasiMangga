#include "kernel-header/config.h"
#include "std-header/std_stringio.h"
#include "shell-header/shell_common.h"
#include "basic-header/std_opr.h"
#include "std-header/boolean.h"

void rm(char *dirtable, char current_dir_index, char flags, char *target);

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
    if (argc == 2 || argc == 3) {
        if (!strcmp("-r", arg_vector[0]))
            rm(directory_table, current_dir_index, 1, arg_vector[1], arg_vector[2]);
        else
            rm(directory_table, current_dir_index, 0, arg_vector[0], arg_vector[1]);
    }
    else
        print("Usage : rm [-r] <source>\n", BIOS_WHITE);

    setShellCache(shell_cache);
    shellReturn();
}

void file_delete(char *file_read, char *source_directory_name, char *returncode_src, char source_dir_idx, char *dirtable) {
    // char as character
    char filename_buffer[16];
    // char as 1 byte integer
    char target_files_entry_index;
    char target_entry_byte;
    char target_parent_byte;
    char link_status = -1;
    bool is_found_parent = false;
    int i = 0;

    if (file_read[0] != NULL) {

        // Find entry in files
        i = 0;
        while (i < FILES_ENTRY_COUNT && !is_found_parent) {
            clear(filename_buffer, 16);
            strcpybounded(filename_buffer, dirtable+FILES_ENTRY_SIZE*i+PATHNAME_BYTE_OFFSET, 14);
            if (dirtable[i*FILES_ENTRY_SIZE + PARENT_BYTE_OFFSET] == source_dir_idx &&
                !strcmp(source_directory_name, filename_buffer)) {
                is_found_parent = true;
                target_entry_byte = dirtable[i*FILES_ENTRY_SIZE+ENTRY_BYTE_OFFSET];
                target_files_entry_index = i;
                link_status = dirtable[i*FILES_ENTRY_SIZE+LINK_BYTE_OFFSET];
            }
            i++;
        }

        if (link_status == HARDLINK_ENTRY || link_status == SOFTLINK_ENTRY) {
            dirtable[target_files_entry_index*FILES_ENTRY_SIZE+ENTRY_BYTE_OFFSET] = EMPTY_FILES_ENTRY;
            dirtable[target_files_entry_index*FILES_ENTRY_SIZE+PARENT_BYTE_OFFSET] = ROOT_PARENT_FOLDER;
            clear(dirtable+target_files_entry_index*FILES_ENTRY_SIZE+PATHNAME_BYTE_OFFSET, 14);

            if (!is_found_parent) {
                print("rm: searching hardlink: ", BIOS_WHITE);
                print(source_directory_name, BIOS_WHITE);
                print(" error\n", BIOS_WHITE);
                returncode_src = -1;
            }
            else if (link_status == HARDLINK_ENTRY) {
                // Updating directory table
                print("rm: hardlink: ", BIOS_WHITE);
                print(source_directory_name, BIOS_WHITE);
                print(" deleted\n", BIOS_WHITE);

                directSectorWrite(dirtable, FILES_SECTOR);
                directSectorWrite(dirtable+SECTOR_SIZE, FILES_SECTOR+1);
            }
            else if (link_status == SOFTLINK_ENTRY) {
                // Updating directory table
                print("rm: softlink: ", BIOS_WHITE);
                print(source_directory_name, BIOS_WHITE);
                print(" deleted\n", BIOS_WHITE);

                directSectorWrite(dirtable, FILES_SECTOR);
                directSectorWrite(dirtable+SECTOR_SIZE, FILES_SECTOR+1);
            }
        }
        else {
            remove(source_directory_name, &returncode_src, source_dir_idx);
            if (returncode_src == -1)
            print("rm: removal error\n", BIOS_LIGHT_RED);
            else {
                print("rm: ", BIOS_WHITE);
                print(source_directory_name, BIOS_WHITE);
                print(" deleted\n", BIOS_WHITE);
            }
        }
    }
    else {
        print("rm: error: ", BIOS_WHITE);
        print(source_directory_name, BIOS_WHITE);
        print(" is a folder\n", BIOS_WHITE);
    }
}

void rm(char *dirtable, char current_dir_index, char flags, char *target) {
    // FIXME : Extra, Extra, Currently deleting file that softlinked will have some consistency problem
    // char as string / char
    char filename_buffer[16];
    char source_directory_name[16];
    // char as 1 byte integer
    char file_read[FILE_SIZE_MAXIMUM];
    char target_entry_byte = 0;
    char stack_but_without_typedef_because_im_scared_with_bcc[256]; // <3 stack, but not bcc :(
    char stack_top_pointer;
    char source_dir_idx;
    int returncode_src = 0;
    bool is_write_success = false, valid_filename = true;
    bool f_target_found = false, empty_entry_found = false;
    int i = 0, j = 0;
    int f_entry_idx = 0;
    int f_entry_sector_idx = 0;
    int last_slash_index;

    // Relative pathing
    clear(source_directory_name, 16);
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



    // Copying file if path evaluation success
    if (returncode_src == -1) {
        print("rm: ", BIOS_GRAY);
        print(target, BIOS_GRAY);
        print(": target not found\n", BIOS_GRAY);
    }
    else {
        clear(file_read, FILE_SIZE_MAXIMUM);
        read(file_read, source_directory_name, &returncode_src, source_dir_idx);
        if (returncode_src == 0) {
            if (flags == 0) {
                // Simple delete, refuse folder
                file_delete(file_read, source_directory_name, returncode_src, source_dir_idx, dirtable);
            }
            else {
                // Recursive
                // Will deleting file normally on -r
                if (file_read[0] != NULL)
                    file_delete(file_read, source_directory_name, returncode_src, source_dir_idx, dirtable);
                else {
                    clear(stack_but_without_typedef_because_im_scared_with_bcc, 256);
                    stack_top_pointer = 0;
                    stack_but_without_typedef_because_im_scared_with_bcc[0] = source_dir_idx;

                }

            }
        }


    }
}
