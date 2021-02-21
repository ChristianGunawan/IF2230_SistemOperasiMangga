// 13519214 - Standard function

int strlen(char* string) {
    // Standard string length function
    int length = 0;
    while (*(string+length) != '\0')
        length++;
    return length;
}
