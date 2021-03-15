// ------- Kernel -------
// BCC notes
// - Variable declaration must be put on top of code
// - First defined function is starting function, not function called main() (Need more testing, either BCC or ld86)
// - Pointer declaration syntax is strictly <type> *<varname>;
// - #include weird behavior if directly next to comment (only 1 newline, 2 newline work fine)
// - Bizarre filename behavior (cannot using filename starting with i (?)), no error & compiled normally but cannot start

#include "kernel-header/kernel.h"
#include "kernel-header/output.h"
#include "kernel-header/screen.h"
#include "std-header/boolean.h"
#include "std-header/std.h"

int main() {
    char buf[1024]; // DEBUG
    char tp[8];
    int temp = 0;
    clear(buf,1024);
    // Setup
    makeInterrupt21();

    // Initial screen
    clearScreen();
    // DEBUG
    // drawBootLogo();     // Note : drawBootLogo() does not revert video mode

    // Change video mode and spawn shell
    interrupt(0x10, 0x0003, 0, 0, 0);

    writeFile(NULL, "folder1", &temp, ROOT_PARENT_FOLDER);
    writeFile(NULL, "fol r2", &temp, 0);
    writeFile(NULL, "hoho    ow", &temp, 1);
    writeFile(NULL, "fo er4", &temp, 2);
    writeFile(NULL, "uwu", &temp, ROOT_PARENT_FOLDER);
    writeFile(NULL, "e 5s a", &temp, 3);
    writeFile("ACTUAL FILE", "f int", &temp, 3);
    writeFile(NULL, "- owo -", &temp, ROOT_PARENT_FOLDER);
    writeFile(NULL, "f r6", &temp, 4);

    shell();
    while (true);
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    char AH = AX >> 8, AL = AX & 0xFF;
    switch (AL) {
        case 0x0:
            switch (CX) {
                case 0x0:
                    printColoredString(BX, DX);
                    break;
                case 0x1:
                    setCursorPos(getCursorPos(1), getCursorPos(0) - 1);
                    directCharPrint(CHAR_SPACE, BIOS_GRAY);
                    break;
            }
            break;
        case 0x1:
            switch (CX) {
                case 0x0:
                    readString(BX);
                    break;
                case 0x1:
                    getFullKeyWrapper(BX);
                    break;
                case 0x2:
                    setCursorPos(BX & 0xFF00, BX & 0x00FF);
                    break;
                case 0x3:
                    getCursorPosWrapper(BX, DX);
                    break;
                default:
                    printString("Invalid interrupt\n");
            }
            break;
        case 0x2:
            readSector(BX, CX);
            break;
        case 0x3:
            writeSector(BX, CX);
            break;
        case 0x4:
            readFile(BX, CX, DX, AH);
            break;
        case 0x5:
            writeFile(BX, CX, DX, AH);
            break;
        default:
            printString("Invalid interrupt\n");
    }
}

void printString(char *string) {
    printColoredString(string, BIOS_GRAY);
}

void readString(char *string) {
    // FIXME : Extra, Literally gets()
    char c, scancode;
    int i = 0, j = 0, max_i = 0, savedCursorRow, savedCursorCol, rawKey;
    enableKeyboardCursor();
    savedCursorRow = getCursorPos(1);
    savedCursorCol = getCursorPos(0);

    do {
        rawKey = getFullKeyPress();
        c = rawKey & 0xFF;      // AL Value
        scancode = rawKey >> 8; // AH Value
        // Warning : Prioritizing ASCII before scancode
        switch (c) {
            case CHAR_INPUT_NEWLINE:
                break;
            case CHAR_BACKSPACE:
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
                setCursorPos(savedCursorRow, savedCursorCol);
                printString(string);

                setCursorPos(savedCursorRow, savedCursorCol + i);
                break;
            default:
                // If char (AL) is not ASCII Control codes, check scancode (AH)
                switch (scancode) {
                    case SCANCODE_DOWN_ARROW:
                    case SCANCODE_UP_ARROW:
                        // Ignored
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
                        directCharPrint(c, BIOS_GRAY);
                        string[i] = c;
                        if (i == max_i)
                            max_i++;
                        i++;
                }
                setCursorPos(savedCursorRow, savedCursorCol + i);
        }
    } while (c != CHAR_INPUT_NEWLINE);
    string[max_i] = CHAR_NULL; // Terminating string
    disableKeyboardCursor();
    setCursorPos(savedCursorRow + 1, 0);
}

// I/O System Calls
// WARNING : It will read entire 512 bytes starting from buffer,
//           ensure entire buffer is cleared first and not reading directly from .rodata
// Note : BIOS counting sector 0 as sector 1 in INT 13H
// Note : Sector 0 is bootloader and 1-KERNEL_SIZE is kernel
void readSector(char *buffer, int sector) {
    interrupt(0x13, 0x0201, buffer, (div(sector, 36) << 8) + mod(sector, 18) + 1, mod(div(sector, 18), 2) << 8);
}

void writeSector(char *buffer, int sector) {
    interrupt(0x13, 0x0301, buffer, (div(sector, 36) << 8) + mod(sector, 18) + 1, mod(div(sector, 18), 2) << 8);
}

void readFile(char *buffer, char *path, int *result, char parentIndex) {
    char files_buf[2][SECTOR_SIZE], sectors_buf[SECTOR_SIZE]; // Filesystem buffer
    char file_segment_buffer[SECTOR_SIZE];
    char filename_buffer[16];
    int i = 0, j = 0;
    int sectors_entry_idx = 0, sector_read_target = 0;
    bool valid_parent_folder = true, is_filename_match_found = false, valid_filename_length = true;

    readSector(files_buf[0], FILES_SECTOR);
    readSector(files_buf[1], FILES_SECTOR + 1);

    // Filename length check
    if (strlen(path) > 14)
        valid_filename_length = false;

    // Find matching filename
    if (valid_filename_length) {
        while (i < 2 && !is_filename_match_found) {
            j = 0;
            while (j < SECTOR_SIZE && !is_filename_match_found) {
                // Checking existing filename in same parent folder
                if (files_buf[i][j+PARENT_BYTE_OFFSET] == parentIndex) {
                    // Needed buffer because entry may ignoring null terminator
                    clear(filename_buffer, 16);
                    strcpybounded(filename_buffer, files_buf[i]+j+PATHNAME_BYTE_OFFSET, 14);
                    if (!strcmp(path, filename_buffer)) {
                        is_filename_match_found = true;
                        sectors_entry_idx = files_buf[i][j+ENTRY_BYTE_OFFSET];
                    }
                }
                j += 0x10;
            }
            i++;
        }
    }

    if (is_filename_match_found) {
        readSector(sectors_buf, SECTORS_SECTOR);
        i = 0;
        sector_read_target = sectors_buf[sectors_entry_idx*0x10 + i];
        while (i < 0x10 && sector_read_target != EMPTY_SECTORS_ENTRY) {
            clear(file_segment_buffer, SECTOR_SIZE);
            readSector(file_segment_buffer, sector_read_target);
            rawstrcpybounded((buffer+i*SECTOR_SIZE), file_segment_buffer, SECTOR_SIZE);
            i++;
            sector_read_target = sectors_buf[sectors_entry_idx*0x10 + i];
        }
    }




    // Error code writing
    if (!is_filename_match_found)
        (*result) = -1;
    else
        (*result) = 0;
}

void writeFile(char *buffer, char *path, int *sectors, char parentIndex) {
    char map_buf[SECTOR_SIZE], files_buf[2][SECTOR_SIZE], sectors_buf[SECTOR_SIZE]; // Filesystem buffer
    char file_segment_buffer[SECTOR_SIZE]; // Buffer for writing to sector, always get clear()
    char filename_buffer[16], adjusted_path[16];
    char parent_entry_byte; // Temporary "P" byte holder / parent index at files filesystem
    int i = 0, j = 0, segment_idx = 0; // Iterator index
    int f_entry_idx = 0, f_entry_sector_idx = 0, sectors_entry_idx = 0; // Targets Index
    int map_empty_bytes_sum = 0, buffer_size = 0, write_file_error_code = 0;
    bool is_empty_dir_exist = false, is_enough_sector = false;
    bool is_empty_sectors_idx_exist = false, is_empty = true;
    bool is_ready_to_write_file = false, is_done_write_file = false;
    bool buffer_type_is_file = true, is_file_already_exist = false;
    bool f_target_found = false;
    bool valid_parent_folder = true, valid_filename = true, valid_filename_length = true;

    // Filename length check
    if (strlen(path) > 14) {
        valid_filename_length = false;
        valid_filename = false;
    }

    // Directory checking in files filesystem
    // valid_filename_length will represent validity of filename length
    // while valid_filename representing whether file / folder is duplicate in single parent
    if (valid_filename_length) {
        readSector(files_buf[0], FILES_SECTOR);
        readSector(files_buf[1], FILES_SECTOR + 1);
        while (i < 2 && valid_filename) {
            j = 0;
            while (j < SECTOR_SIZE && valid_filename) {
                // Checking entry byte flag ("S" byte)
                if (files_buf[i][j+ENTRY_BYTE_OFFSET] == EMPTY_FILES_ENTRY && !f_target_found) {
                    f_entry_sector_idx = i;
                    f_entry_idx = j;
                    f_target_found = true; // If empty dir entry exists, stop find new one (Pick first empty entry)
                    is_empty_dir_exist = true;
                }
                // Checking existing filename in same parent folder
                if (files_buf[i][j+PARENT_BYTE_OFFSET] == parentIndex) {
                    // Needed buffer because entry may ignoring null terminator
                    clear(filename_buffer, 16);
                    strcpybounded(filename_buffer, files_buf[i]+j+PATHNAME_BYTE_OFFSET, 14);
                    if (!strcmp(path, filename_buffer))
                        valid_filename = false;
                }
                j += 0x10;
            }
            i++;
        }
    }

    // Checking buffer type, either writing folder or file
    if (buffer == NULL)
        buffer_type_is_file = false;

    // Checking whether folder located at parentIndex is valid
    // parentIndex == ROOT_PARENT_FOLDER always valid parent folder
    if (parentIndex != ROOT_PARENT_FOLDER) {
        // div(parentIndex,0x20) -> Because 1 files filesystem only contain 0x20 index
        // mod(parentIndex*FILE_SECTOR_SIZE, SECTOR_SIZE)+ENTRY_BYTE_OFFSET ->
        //      2 files filesystem span from 0 to 2*SECTOR_SIZE-1 bytes, 1 files only contain 1 SECTOR_SIZE.
        //      ENTRY_BYTE_OFFSET used for checking "S" byte / entry byte in files filesystem
        parent_entry_byte = files_buf[div(parentIndex,0x20)][mod(parentIndex*FILE_SECTOR_SIZE, SECTOR_SIZE)+ENTRY_BYTE_OFFSET];
        if (parent_entry_byte != FOLDER_ENTRY)
            valid_parent_folder = false;
    }

    // Writing file / folder
    if (is_empty_dir_exist && valid_parent_folder && valid_filename) {
        // Updating files filesystem buffer
        files_buf[f_entry_sector_idx][f_entry_idx+PARENT_BYTE_OFFSET] = parentIndex;
        rawstrcpy((files_buf[f_entry_sector_idx]+f_entry_idx+PATHNAME_BYTE_OFFSET), path);

        // ----------- Folder Writing Branch-----------
        // Folder writing does not need to readSector() sectors and map
        if (!buffer_type_is_file) {
            // Updating files filesystem buffer
            files_buf[f_entry_sector_idx][f_entry_idx+ENTRY_BYTE_OFFSET] = FOLDER_ENTRY;
        }


        // ----------- File Writing Branch-----------
        // readSector() for sectors and map will be called if needed
        // Checking whether enough empty space or not in map filesystem
        if (buffer_type_is_file) {
            readSector(map_buf, MAP_SECTOR);
            i = 0;
            buffer_size = strlen(buffer); // In bytes
            while (i < (SECTOR_SIZE >> 1) && !is_enough_sector) {
                // Finding empty sector in map
                if (map_buf[i] == EMPTY_MAP_ENTRY)
                    map_empty_bytes_sum += SECTOR_SIZE;
                if (buffer_size <= map_empty_bytes_sum)
                    is_enough_sector = true;
                i++;
            }
        }

        // Checking available entry in sectors filesystem
        if (is_enough_sector) {
            readSector(sectors_buf, SECTORS_SECTOR);
            // Outer loop checking per files (1 file = 16 bytes in sectors filesystem)
            i = 0;
            while (i < 0x20 && !is_empty_sectors_idx_exist) {
                j = 0;
                is_empty = true;
                // Inner loop checking is 1 file is all 0x00 byte or not
                while (j < FILE_SECTOR_SIZE && is_empty) {
                    if (sectors_buf[i*FILE_SECTOR_SIZE + j] != EMPTY_SECTORS_ENTRY)
                        is_empty = false;
                    j++;
                }

                // If found empty index set flag
                if (is_empty) {
                    is_empty_sectors_idx_exist = true;
                    is_ready_to_write_file = true;
                    sectors_entry_idx = i;
                }

                i++; // Jumping 16 bytes
            }
        }

        // File writing
        if (is_ready_to_write_file) {
            // Updating files filesystem buffer
            files_buf[f_entry_sector_idx][f_entry_idx+ENTRY_BYTE_OFFSET] = sectors_entry_idx;

            // Find empty sector between 0x0 and 0x100
            // (256, limitation of 1 byte entry in sectors filesystem) and write
            i = 0;
            j = 0;
            while (i < (SECTOR_SIZE >> 1) && !is_done_write_file) {
                if (map_buf[i] == EMPTY_MAP_ENTRY) {
                    // Updating map filesystem
                    map_buf[i] = FILLED_MAP_ENTRY;

                    // Updating sectors filesystem
                    // FIXME : Extra, split to multiple sectors
                    // WARNING : Will stop writing if file more than 8192 bytes
                    if (j < 16)
                        sectors_buf[sectors_entry_idx*FILE_SECTOR_SIZE+j] = i;
                    j++;

                    // Entry writing at sector
                    clear(file_segment_buffer, SECTOR_SIZE);
                    rawstrcpybounded(file_segment_buffer, (buffer+segment_idx*SECTOR_SIZE), SECTOR_SIZE);
                    writeSector(file_segment_buffer, i);
                    segment_idx++;
                    buffer_size -= SECTOR_SIZE;
                }
                // Checking is file done writing
                if (buffer_size <= 0)
                    is_done_write_file = true;

                i++;
            }

        }


        // Filesystem records update
        writeSector(files_buf[0], FILES_SECTOR);
        writeSector(files_buf[1], FILES_SECTOR + 1);
        if (buffer_type_is_file) {
            writeSector(map_buf, MAP_SECTOR);
            writeSector(sectors_buf, SECTORS_SECTOR);
        }
    }



    // Error code writing
    if (!valid_filename)
        (*sectors) = -1; // Filename too long or file exists
    else if (!is_empty_dir_exist)
        (*sectors) = -2; // Not enough entry in files filesystem
    else if (!is_enough_sector && buffer_type_is_file)
        (*sectors) = -3; // Not enough empty sectors, only for files
    else if (!valid_parent_folder)
        (*sectors) = -4; // Parent folder not valid
    else
        (*sectors) = 0;
}
