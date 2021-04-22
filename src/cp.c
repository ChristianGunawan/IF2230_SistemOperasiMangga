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

void file_copy(char *file_read, char *source_directory_name, char *returncode_src, char source_dir_idx,
            char *copied_directory_name, char *returncode_cpy, char copied_dir_idx, char *dirtable) {
    char filename_buffer[16];

    char target_entry_byte;
    char link_status = -1;
    char target_parent_byte;
    char original_softlink_index;

    int i = 0;
    bool empty_entry_found = false;
    bool is_found_parent = false;


    // Find entry in files
    i = 0;
    while (i < FILES_ENTRY_COUNT && !is_found_parent) {
        clear(filename_buffer, 16);
        strcpybounded(filename_buffer, dirtable+FILES_ENTRY_SIZE*i+PATHNAME_BYTE_OFFSET, 14);
        if (dirtable[i*FILES_ENTRY_SIZE + PARENT_BYTE_OFFSET] == source_dir_idx &&
            !strcmp(source_directory_name, filename_buffer)) {
            is_found_parent = true;
            target_entry_byte = dirtable[i*FILES_ENTRY_SIZE+ENTRY_BYTE_OFFSET];
            target_parent_byte = dirtable[i*FILES_ENTRY_SIZE+PARENT_BYTE_OFFSET];
            link_status = dirtable[i*FILES_ENTRY_SIZE+LINK_BYTE_OFFSET];
            memcpy(filename_buffer, dirtable+i*FILES_ENTRY_SIZE+PATHNAME_BYTE_OFFSET, 14);
            original_softlink_index = i;
        }
        i++;
    }

    if (link_status == SOFTLINK_ENTRY || link_status == HARDLINK_ENTRY) {
        // Will copy softlink
        getDirectoryTable(dirtable);

        // Find empty entry
        i = 0;
        while (i < FILES_ENTRY_COUNT && !empty_entry_found) {
            if (dirtable[i*FILES_ENTRY_SIZE+ENTRY_BYTE_OFFSET] == EMPTY_FILES_ENTRY) {
                empty_entry_found = true;
                dirtable[i*FILES_ENTRY_SIZE+PARENT_BYTE_OFFSET] = target_parent_byte;
                dirtable[i*FILES_ENTRY_SIZE+ENTRY_BYTE_OFFSET] = target_entry_byte;
                clear(filename_buffer, 16);
                strcpybounded(filename_buffer, copied_directory_name, 14);
                memcpy(dirtable+i*FILES_ENTRY_SIZE+PATHNAME_BYTE_OFFSET, filename_buffer, 14);
                dirtable[i*FILES_ENTRY_SIZE+LINK_BYTE_OFFSET] = link_status;
            }
            i++;
        }

        directSectorWrite(dirtable, FILES_SECTOR);
        directSectorWrite(dirtable+SECTOR_SIZE, FILES_SECTOR+1);
    }
    else {
        if (target_entry_byte != EMPTY_FILES_ENTRY) {
            if (file_read[0] != NULL)
                write(file_read, copied_directory_name, &returncode_cpy, copied_dir_idx);
            else {
                print("cp: error: ", BIOS_WHITE);
                print(source_directory_name, BIOS_WHITE);
                print(" is a folder\n", BIOS_WHITE);
                returncode_cpy = -1;
            }
        }
        else {
        print("cp: ", BIOS_WHITE);
        print(source_directory_name, BIOS_WHITE);
        print(" softlink broken\n", BIOS_WHITE);
    }
    }

}

void cp(char *dirtable, char current_dir_index, char flags, char *target, char *linkname) {
    // Technically just "copy" of previous implementation of ln
    // char as string / char
    char filename_buffer[16];
    char source_directory_name[16];
    char copied_directory_name[16];
    // char as 1 byte integer
    char file_read[FILE_SIZE_MAXIMUM];
    char target_entry_byte = 0;
    char target_parent_byte = 0;
    char link_status = -1;
    char stack_but_without_typedef_because_im_scared_with_bcc[256]; // <3 stack, but not bcc :(
    char stack_top_pointer;
    char source_dir_idx;
    char copied_dir_idx;
    int returncode_src = 0;
    int returncode_cpy = 0;
    bool is_write_success = false, valid_filename = true;
    bool f_target_found = false, empty_entry_found = false;
    bool is_found_parent = false;
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
        read(file_read, copied_directory_name, &returncode_cpy, copied_dir_idx);
        clear(file_read, FILE_SIZE_MAXIMUM);
        read(file_read, source_directory_name, &returncode_src, source_dir_idx);
        if (returncode_src == 0 && returncode_cpy == -1) {
            if (flags == 0) {
                // Simple copy, refuse folder
                if (file_read[0] != NULL)
                    file_copy(file_read, source_directory_name, &returncode_src, source_dir_idx,
                        copied_directory_name, &returncode_cpy, copied_dir_idx, dirtable);
                    // write(file_read, copied_directory_name, &returncode_cpy, copied_dir_idx);
                else {
                    print("cp: error: ", BIOS_WHITE);
                    print(target, BIOS_WHITE);
                    print(" is a folder\n", BIOS_WHITE);
                    returncode_src = -1;
                }
            }
            else {
                // Stolen from rm
                // Will deleting file normally on -r
                if (file_read[0] != NULL) {
                    file_copy(file_read, source_directory_name, &returncode_src, source_dir_idx,
                        copied_directory_name, &returncode_cpy, copied_dir_idx, dirtable);
                }
                else {
                    // clear(stack_but_without_typedef_because_im_scared_with_bcc, 256);
                    // stack_top_pointer = 0;
                    // stack_but_without_typedef_because_im_scared_with_bcc[0] = source_dir_idx;
                    //
                    // while (!recursion_loop_pass_success) {
                    //     i = 0;
                    //     recursion_loop_pass_success = true;
                    //     current_recursion_parent_index = stack_but_without_typedef_because_im_scared_with_bcc[stack_top_pointer];
                    //     while (i < FILES_ENTRY_COUNT) {
                    //         if (current_recursion_parent_index == dirtable[i*FILES_ENTRY_SIZE+PARENT_BYTE_OFFSET]) {
                    //             // If still file, pass is still success
                    //             if (dirtable[i*FILES_ENTRY_SIZE+ENTRY_BYTE_OFFSET] != FOLDER_ENTRY
                    //                 && dirtable[i*FILES_ENTRY_SIZE+ENTRY_BYTE_OFFSET] != EMPTY_FILES_ENTRY) {
                    //                 clear(filename_buffer, 16);
                    //                 strcpybounded(filename_buffer, dirtable+i*FILES_ENTRY_SIZE+PATHNAME_BYTE_OFFSET, 14);
                    //                 file_delete(file_read, filename_buffer, returncode_src, current_recursion_parent_index, dirtable);
                    //             }
                    //             // If folder, pass failed
                    //             else {
                    //                 stack_top_pointer++;
                    //                 stack_but_without_typedef_because_im_scared_with_bcc[stack_top_pointer] = i;
                    //                 recursion_loop_pass_success = false;
                    //             }
                    //         }
                    //         i++;
                    //     }
                    // }
                    //
                    // // Folder stack deletion
                    // // Updating to current updated files filesystem
                    // getDirectoryTable(dirtable);
                    // // Find entry in files
                    // i = 0;
                    // while (i <= stack_top_pointer) {
                    //     stack_folder_idx = stack_but_without_typedef_because_im_scared_with_bcc[i];
                    //     dirtable[stack_folder_idx*FILES_ENTRY_SIZE+PARENT_BYTE_OFFSET] = ROOT_PARENT_FOLDER;
                    //     dirtable[stack_folder_idx*FILES_ENTRY_SIZE+ENTRY_BYTE_OFFSET] = EMPTY_FILES_ENTRY;
                    //     clear(filename_buffer, 16);
                    //     strcpybounded(filename_buffer, dirtable+stack_folder_idx*FILES_ENTRY_SIZE+PATHNAME_BYTE_OFFSET, 14);
                    //
                    //     if (getKeyboardCursor(true) > 20)
                    //         scrollScreen();
                    //
                    //     print("rm: ", BIOS_WHITE);
                    //     print(filename_buffer, BIOS_LIGHT_BLUE);
                    //     print(" deleted\n", BIOS_WHITE);
                    //     clear(dirtable+stack_folder_idx*FILES_ENTRY_SIZE+PATHNAME_BYTE_OFFSET, 14);
                    //     i++;
                    // }
                    //
                    // // Updating files filesystem entry
                    // directSectorWrite(dirtable, FILES_SECTOR);
                    // directSectorWrite(dirtable+SECTOR_SIZE, FILES_SECTOR+1);

                    print("rm: recursion done\n", BIOS_WHITE);
                }
            }

        }
        else {
            print("cp: error: ", BIOS_WHITE);
            print(target, BIOS_WHITE);
            print(" not found\n", BIOS_WHITE);
            returncode_src = -1;
        }

        if (returncode_src == 0 && returncode_cpy == -1) {
            print(linkname, BIOS_GRAY);
            print(": copy created\n", BIOS_GRAY);
        }
        else
            print("cp: file writing error\n", BIOS_GRAY);
    }
}
