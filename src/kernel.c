// ------- Kernel -------


// Configuration
#define V_MEM 0xB000
#define V_OFFSET 0x8000


// Implemented in assembly
extern void putInMemory(int segment, int address, char character);
extern void makeInterrupt21();
extern int interrupt(int number, int AX, int BX, int CX, int DX);

void handleInterrupt21 (int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
void clear(char *buffer, int length); //Fungsi untuk mengisi buffer dengan 0
void videoMemoryWrite(char char);

int main() {
    // TODO : Extra, ASCII
    // printString("halooo");
    int i = 0;
    while (i < 15) {
        printString("PPPPPPPPPPPPP");
        i++;
    }

    // Temporary read string
    while (1) {
        int s = interrupt(0x16, 0x00, 0, 0, 0);
        if (s == 8) // Backspace
            videoMemoryWrite(0);
        else
            videoMemoryWrite(s);
    }

    // interrupt(0x10, 0x1300, 0x010D, 0x0005 ,0x0101);
    // makeInterrupt21();

    while (1);
}

void handleInterrupt21(int AX, int BX, int CX, int DX){
    switch (AX) {
        // case 0x0:
        //     printString(BX);
        //     break;
        // case 0x1:
        //     // readString(BX);
        //     break;
        default:
            printString("Invalid interrupt");
    }
}

int strlen(char* string) {
    // Standard string length function
    int length = 0;
    while (*(string+length) != '\0')
        length++;
    return length;
}

void videoMemoryWrite(char char) {
    // putInMemory() wrapper for video memory
    putInMemory(V_MEM, V_OFFSET, char);
}

void printString(char *string) {
    // Self-note : (?) pt[i] operator not translate to *(pt+i) in bcc
    int i = 0;
    while (*(string+i) != '\0') {
        videoMemoryWrite(string[i]);
        videoMemoryWrite(0xD);
        i++;
    }

    // interrupt(0x10,0x1301,0x010D,strlen(string),0);
}





// void readString(char *string);
// void clear(char *buffer, int length); //Fungsi untuk mengisi buffer dengan 0
