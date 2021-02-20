// ------- Kernel -------


// Configuration
#define V_MEM 0xB000
#define V_OFFSET 0x8000


// Implemented in assembly
extern void putInMemory(int segment, int address, char character);
extern int interrupt (int number, int AX, int BX, int CX, int DX);

// void printString(char *string) {
    // int i = 0;
    // while (string[i] != '\0') {
    //     putInMemory(V_MEM, V_OFFSET + 2*i, string[i]);
    //     putInMemory(V_MEM, V_OFFSET + 1 + 2*i, 0xD);
    //     i++;
    // }
    // interrupt(0x10,0x1301,0x010D,0x5,0);
// }


int main () {
    // TODO : Extra, ASCII
    // printString("Hello");

    putInMemory(V_MEM, 0x8000, 'H');
    putInMemory(V_MEM, 0x8001, 0xC);
    putInMemory(V_MEM, 0x8002, 'a');
    putInMemory(V_MEM, 0x8003, 0xC);
    putInMemory(V_MEM, 0x8004, 'i');
    putInMemory(V_MEM, 0x8005, 0xC);

    while (1);
}


void handleInterrupt21 (int AX, int BX, int CX, int DX){
    // switch (AX) {
    //     case 0x0:
    //         printString(BX);
    //         break;
    //     case 0x1:
    //         readString(BX);
    //         break;
    //     default:
    //         printString("Invalid interrupt");
    // }
}


// void readString(char *string);
// void clear(char *buffer, int length); //Fungsi untuk mengisi buffer dengan 0
