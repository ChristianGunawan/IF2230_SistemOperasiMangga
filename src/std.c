// 13519214 - Standard function

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

// TODO : print()
