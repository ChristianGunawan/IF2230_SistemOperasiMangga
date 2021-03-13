// 13519214 - Standard function header

// Standard string operation
int strlen(char *string);
// Standard string length

int mod(int a, int n);
// Standard modulo function

void strcpy(char *dest, char *src);
// Standard strcpy without returning

char strcmp(char *s1, char *s2);
// Standard strcmp function

void strrev(char *string);
// Reversing string at pointed location

void inttostr(char *buffer, int n);
// WARNING : Naive implementation, no bound checking
// Converting integer n to string pointed at buffer



// Standard I/O
void print(char *string, char color);
// Simple printing with color

void gets(char *string);
// Simple keyboard input, ye olde gets()

void putchar(char a);
// Standard 1 char output

int getFullKey();
// Getting 1 keypress, blocking, no echo
