#include "kernel-header/config.h"
#include "std-header/std_stringio.h"
#include "shell-header/shell_common.h"
#include "basic-header/std_opr.h"

void cat(char *dirtable, char *filename, char target_dir);

int main() {
    char directory_table[FILES_SECTOR_SIZE*SECTOR_SIZE];
    char shell_cache[SECTOR_SIZE];
    char dirstr[BUFFER_SIZE];
    char argc = 0;
    int returncode = 0;
    char current_dir_index;

    clear(shell_cache, SECTOR_SIZE);
    getDirectoryTable(directory_table);
    getShellCache(shell_cache);

    clear(dirstr, BUFFER_SIZE);
    memcpy(dirstr, shell_cache+ARGV_OFFSET, ARG_LENGTH);
    argc = shell_cache[ARGC_OFFSET];
    current_dir_index = shell_cache[CURRENT_DIR_CACHE_OFFSET];


    // Argument count
    if (argc == 2)
        cat(directory_table, dirstr, current_dir_index);
    else
        print("Usage : cat <filename>\n", BIOS_WHITE);
    shellReturn();
}


void cat(char *dirtable, char *filename, char target_dir) {
    char file_read[FILE_SIZE_MAXIMUM];
    char directory_name[ARGC_MAX][ARG_LENGTH];
    char evaluator_temp[ARGC_MAX*ARG_LENGTH];
    char eval_dir = target_dir;
    int returncode = -1, eval_returncode = -1;
    int dirnamecount;
    int i, j, k;


    // Another splitter stealing
    clear(directory_name, ARGC_MAX*ARG_LENGTH);
    clear(evaluator_temp, ARGC_MAX*ARG_LENGTH);
    i = 0;
    j = 0;
    k = 0;
    while (filename[i] != CHAR_NULL) {
        // If found slash in commands and not within double quote mark, make new
        if (filename[i] == CHAR_SLASH && j < ARGC_MAX) {
            k = 0;
            j++;
        }
        else {
            // Only copy if char is not double quote
            // and space outside double quote
            directory_name[j][k] = filename[i];
            k++;
        }
        i++;
    }
    dirnamecount = j + 1; // Due j is between counting space between 2 args

    i = 0;
    // Note : Entry directory_name[dirnamecount - 1] is a FILENAME, not folder
    while (i < dirnamecount - 1) {
        strapp(evaluator_temp, directory_name[i]);
        strapp(evaluator_temp, "/");
        i++;
    }

    if (dirnamecount > 1)
        eval_dir = directoryEvaluator(dirtable, evaluator_temp, &eval_returncode, target_dir);

    clear(file_read, FILE_SIZE_MAXIMUM);
    // Take last argv, use it as filename
    read(file_read, directory_name[dirnamecount-1], &returncode, eval_dir);
    if (returncode == -1) {
        print("cat: ", BIOS_GRAY);
        print(directory_name[dirnamecount-1], BIOS_GRAY);
        print(": file not found", BIOS_GRAY);
    }
    else {
        if (file_read[0] == NULL) {
            print("cat: ", BIOS_WHITE);
            print(directory_name[dirnamecount-1], BIOS_WHITE);
            print(" is a folder", BIOS_WHITE);
        }
        else {
            i = 0;
            while (i < FILE_SIZE_MAXIMUM && file_read[i] != CHAR_NULL) {
                if (file_read[i] == CHAR_CARRIAGE_RETURN)
                    file_read[i] = CHAR_SPACE;
                i++;
            }
            print(file_read, BIOS_GRAY);
        }
    }
    print("\n", BIOS_GRAY);
}
