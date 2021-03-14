// 13519214 - Configuration
#define V_MEM 0xB000
#define V_OFFSET 0x8000
#define V_GRAPHIC 0xA000
#define CHAR_INPUT_NEWLINE CHAR_CARRIAGE_RETURN

// ASCII Charset macro
#define CHAR_LINEFEED 0xA
#define CHAR_CARRIAGE_RETURN 0xD
#define CHAR_BACKSPACE 0x8
#define CHAR_NULL 0x0
#define CHAR_SPACE 0x20
#define CHAR_NUMBER_0 0x30

// BIOS Scancodes
#define SCANCODE_RIGHT_ARROW 0x4D
#define SCANCODE_LEFT_ARROW 0x4B
#define SCANCODE_UP_ARROW 0x48
#define SCANCODE_DOWN_ARROW 0x50

// BIOS Color Enum
// Note : Probably bcc doesn't have enum keyword
#define BIOS_BLACK 0x0
#define BIOS_BLUE 0x1
#define BIOS_GREEN 0x2
#define BIOS_CYAN 0x3
#define BIOS_RED 0x4
#define BIOS_MAGENTA 0x5
#define BIOS_BROWN 0x6
#define BIOS_GRAY 0x7

#define BIOS_DARK_GRAY 0x8
#define BIOS_LIGHT_BLUE 0x9
#define BIOS_LIGHT_GREEN 0xA
#define BIOS_LIGHT_CYAN 0xB
#define BIOS_LIGHT_RED 0xC
#define BIOS_LIGHT_MAGENTA 0xD
#define BIOS_YELLOW 0xE
#define BIOS_WHITE 0xF


// --- Internal Filesystem Configuration ---
// ~ Root directory is located at "/"
// ~ Empty sector in map filesystem is equal with EMPTY_MAP_ENTRY
// ~ Empty entry in files filesystem is flagged by S byte
//         equal EMPTY_FILES_ENTRY and P byte equal ROOT_PARENT_FOLDER
// ~ Empty entry in sectors filesystem if and only if entire 16 bytes
//         is equal with EMPTY_SECTORS_ENTRY

// Bytes count in 1 sector
#define SECTOR_SIZE 512

// Kernel Maximum Size, if changed ensure dd commands in makefile,
//                      bootloader.asm, and fscreator set properly
#define KERNEL_SECTOR_SIZE 31

// Macro for filesystem syscalls
#define FILE_SECTOR_SIZE 0x10 // 16 sectors (8192 bytes) for 1 file entry
#define MAP_SECTOR 0x100
#define FILES_SECTOR 0x101
#define SECTORS_SECTOR 0x103

// Predefined values in map filesystem
#define EMPTY_MAP_ENTRY 0x00 // For empty entry
#define FILLED_MAP_ENTRY 0xFF // If sector are filled

// Flags in files filesystem
#define ROOT_PARENT_FOLDER 0xFF // Flag for "P" byte
#define EMPTY_FILES_ENTRY 0xFE // Flag for "S" byte
#define FOLDER_ENTRY 0xFF // Flag for "S" byte
#define PARENT_BYTE_OFFSET 0x0 // "P" byte, parent folder index
#define ENTRY_BYTE_OFFSET 0x1 // "S" byte, entry index at sectors filesystem
#define PATHNAME_BYTE_OFFSET 0x2 // 14 bytes, filled with pathnames

// Predefined values in sectors filesystem
#define EMPTY_SECTORS_ENTRY 0x00 // For empty entry
