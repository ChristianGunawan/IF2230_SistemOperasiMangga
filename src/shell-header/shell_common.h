// Commonly used function or procedures in shell
#define CURRENT_DIR_CACHE_OFFSET 0xF
#define HISTORY_CACHE_OFFSET 0x10
#define CACHE_SIGNATURE_OFFSET 0x0
#define ARGV_OFFSET 0x150
#define ARGC_OFFSET 0xE

#define LS_TARGET_DIR_CACHE_OFFSET 1
#define CD_TARGET_DIR_CACHE_OFFSET 1

#define CACHE_SIGNATURE 'm'

#define ARG_LENGTH 32
#define ARGC_MAX 8
#define BUFFER_SIZE 64
#define MAX_HISTORY 5

void getDirectoryTable(char *buffer);
// WARNING : No bound checking
// Get all directory table, put in buffer

void shellReturn();
// Exec shell on segment 0x2000

void getShellCache(char *buffer);
// Get shell cache, used for message passing and state storage

void setShellCache(char *buffer);
// Set shell cache, used for updating cache

char directoryEvaluator(char *dirtable, char *dirstr, int *returncode, char current_dir);
// Directory string evaluator, returning evaluated files filesystem entry index
