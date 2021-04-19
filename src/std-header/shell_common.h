// Commonly used function or procedures in shell
#define CURRENT_DIR_CACHE_OFFSET 0
#define HISTORY_CACHE_OFFSET 0x10

#define LS_TARGET_DIR_CACHE_OFFSET 1

void getDirectoryTable(char *buffer);
// WARNING : No bound checking
// Get all directory table, put in buffer

void shellReturn();
// Exec shell on segment 0x2000

void getShellCache(char *buffer);
// Get shell cache, used for message passing and state storage

void setShellCache(char *buffer);
// Set shell cache, used for updating cache
