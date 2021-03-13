// 13519214 - Standard function

#include "kernel-header/config.h"

int strlen(char *string) {
    // Standard string length function
    int i = 0;
    while (string[i] != '\0')
        i++;
    return i;
}

void strcpy(char *dest, char *src) {
    int i = 0;
    while (src[i] != '\0')
        dest[i] = src[i];
}

char strcmp(char *s1, char *s2) {
    int i = 0;
    if (strlen(s1) == strlen(s2)) {
        // If string length matches, check every char
        while (s1[i] != '\0') {
            if (s1[i] != s2[i])
                return 1;
        }

        // If both string matches
        return 0;
    }

    return 1;
}



void print(char *string, char color) {
    // TODO : Extra, Maybe not safe (?)
    // TODO : Extra, Including black color
    if (BIOS_BLACK < color && color <= BIOS_WHITE)
        interrupt(0x21, 0x00, string, 0x00, color);
    else
        interrupt(0x21, 0x00, string, 0x00, BIOS_GRAY);
}

void gets(char *string) {
    interrupt(0x21, 0x01, string, 0x00, 0);
}

void putchar(char a) {
    int temp;
    char tempstring[2];
    tempstring[0] = a;
    tempstring[1] = '\0';
    if (a == CHAR_BACKSPACE)
        interrupt(0x21, 0x00, 0, 0x01, 0);
    else
        interrupt(0x21, 0x00, tempstring, 0x00, BIOS_GRAY);

}

int getFullKey() {
    int key;
    interrupt(0x21, 0x01, &key, 0x01, 0);
    return key;
}

// TODO : getRawCursorPos() wrapper
// TODO : setCursorPos() wrapper


// TODO : Extra, printf(), Extra Extra : Color escape sequence

// TODO : Extra, strtoint / atoi and inttostr
