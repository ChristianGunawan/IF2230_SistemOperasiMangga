// 13519214 - Standard function header

// ---------------- Standard Macro ----------------
#define NULL 0x00



// ---------------- Standard string operation ----------------
int strlen(char *string);
// Standard string length

int mod(int a, int n);
// Standard modulo function

void strcpy(char *dest, char *src);
// Standard strcpy without returning

void strcpybounded(char *dest, char *src, int n);
// strcpy only first n characters

void rawstrcpy(char *dest, char *src);
// strcpy without copying null terminator

char strcmp(char *s1, char *s2);
// Standard strcmp function

void strrev(char *string);
// Reversing string at pointed location

void inttostr(char *buffer, int n);
// WARNING : Naive implementation, no bound checking
// Converting integer n to string pointed at buffer



// ---------------- Standard I/O ----------------
void print(char *string, char color);
// Simple printing with color

void gets(char *string);
// Simple keyboard input, ye olde gets()

void putchar(char a);
// Standard 1 char output

int getFullKey();
// Getting 1 keypress, blocking, no echo
