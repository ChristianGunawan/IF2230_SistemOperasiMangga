// ------- Kernel -------
// BCC notes
// - Variable declaration must be put on top of code
// - First defined function is starting function
// - Pointer declaration syntax is <type> *<varname>;
// - #include weird behavior if directly next to comment (only 1 newline, 2 newline work fine)
// - Bizzare filename behavior (cannot using filename starting with i (?)), no error & compiled normally but cannot start

#include "kernel-header/kernel.h"
#include "kernel-header/output.h"
#include "kernel-header/screen.h"
#include "std-header/boolean.h"
#include "std-header/std.h"

int main() {
    char buf[1024];
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

    // FS DEBUGGING
    writeFile("Entry kok byte gan", "Ininamanyafile", &temp, ROOT_PARENT_FOLDER);
    inttostr(buf, temp);
    print(buf);
    print(": File return code\n");
    writeFile(NULL, "Folder gan", &temp, ROOT_PARENT_FOLDER);
    inttostr(buf, temp);
    print(buf);
    print(": Root folder return code\n");
    writeFile(NULL, "Folder gagal", &temp, 2);
    inttostr(buf, temp);
    print(buf);
    print(": Folder on fake folder return code\n");
    writeFile(NULL, "Folder anak", &temp, 1);
    inttostr(buf, temp);
    print(buf);
    print(": Folder on folder return code\n");
    
    shell();
    while (true);
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    switch (AX) {
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

void clear(char *string, int length) {
    int i = 0;
    while (i < length) {
        string[i] = CHAR_NULL;
        i++;
    }
}

// I/O System Calls
// WARNING : It will read entire 512 bytes starting from buffer,
//           ensure entire buffer is cleared first and not reading directly from .rodata
// Note : BIOS counting sector 0 as sector 1 in INT 13H
// Note : Sector 0 is bootloader and 1-10 is kernel
void readSector(char *buffer, int sector) {
    interrupt(0x13, 0x0201, buffer, (div(sector, 36) << 8) + mod(sector, 18) + 1, mod(div(sector, 18), 2) << 8);
}

void writeSector(char *buffer, int sector) {
    // TODO : Read more, CH = Track on AH 03H (?)
    interrupt(0x13, 0x0301, buffer, (div(sector, 36) << 8) + mod(sector, 18) + 1, mod(div(sector, 18), 2) << 8);
}

void readFile(char *buffer, char *path, int *result, char parentIndex);
// Read file with relative path

void writeFile(char *buffer, char *path, int *sectors, char parentIndex) {
    // Error code list
    // 0 - Exit successfully
    // -1 - File exists
    // -2 - Not enough entry in files filesystem
    // -3 - Not enough empty sectors
    // -4 - Invalid folder
    char map_buf[SECTOR_SIZE], files_buf[2][SECTOR_SIZE], sectors_buf[SECTOR_SIZE]; // Filesystem buffer
    char file_segment_buffer[SECTOR_SIZE]; // Buffer for writing to sector, always get clear()
    char dbg[16]; // DEBUG
    int idbg; // DEBUG
    char filename_buffer[16], adjusted_path[16];
    char parent_entry_byte;
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
                    f_target_found = true;
                    is_empty_dir_exist = true;
                }
                // Checking existing filename in same parent folder
                // if (files_buf[i][j+PARENT_BYTE_OFFSET] != ROOT_PARENT_FOLDER) {
                //     inttostr(dbg, files_buf[i][j+PARENT_BYTE_OFFSET]);
                //     print(dbg);
                // }
                if (files_buf[i][j+PARENT_BYTE_OFFSET] == parentIndex) {
                    // Needed buffer because entry may ignoring null terminator
                    clear(filename_buffer, 16);
                    strcpybounded(filename_buffer, files_buf[i]+j+PATHNAME_BYTE_OFFSET, 14);

                    if (!strcmp(path, filename_buffer)) // FIXME : never executed
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
    // parentIndex equal ROOT_PARENT_FOLDER always valid parent folder
    if (parentIndex != ROOT_PARENT_FOLDER) {
        // div(parentIndex,0x20) -> Because 1 files filesystem only contain 0x20 index
        // mod(parentIndex*0x10, SECTOR_SIZE)+ENTRY_BYTE_OFFSET ->
        //      2 files filesystem span from 0 to 2*SECTOR_SIZE-1 bytes, 1 files only contain 1 SECTOR_SIZE.
        //      ENTRY_BYTE_OFFSET used for checking "S" byte / entry byte in files filesystem
        parent_entry_byte = files_buf[div(parentIndex,0x20)][mod(parentIndex*0x10, SECTOR_SIZE)+ENTRY_BYTE_OFFSET];
        if (parent_entry_byte == EMPTY_FILES_ENTRY)
            valid_parent_folder = false;
    }

    // Writing a file
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
            while (i < SECTOR_SIZE && !is_enough_sector) {
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
                while (j < 0x10 && is_empty) {
                    if (sectors_buf[i*0x10 + j] != EMPTY_SECTORS_ENTRY)
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
                        sectors_buf[sectors_entry_idx*0x10+j] = i;
                    j++;

                    // Entry writing at sector
                    clear(file_segment_buffer, SECTOR_SIZE);
                    strcpy(file_segment_buffer, (buffer+segment_idx*SECTOR_SIZE));
                    writeSector(file_segment_buffer, i);
                    // DEBUG
                    inttostr(dbg, i);
                    print("Sector : ");
                    print(dbg, BIOS_GREEN);
                    print("\nValue : ");
                    print(file_segment_buffer);
                    print("\n");
                    // END
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
    else if (!valid_parent_folder)
        (*sectors) = -4; // Parent folder not valid
    else if (!is_enough_sector && buffer_type_is_file)
        (*sectors) = -3; // Not enough empty sectors
    else
        (*sectors) = 0;
    // else if (!) // TODO : Completion
}
