Cara Kerja Interupt
Memotong/menyela suatu proses yang sedang berjalan dan memberitahu CPU bahwa akan dilakukan pemanggilan system call atau fitur-fitur bersifat low-level tertentu.

Cara Kerja kernel.asm
kernel.asm berfungsi menyediakan instruksi-instruksi mesin yang dapat dipakai dalam pembuatan kernel operating system. Instruksi-instruksi level mesin dalam bahasa assembly itu dibungkus menjadi fungsi-fungsi yang dapat di-import dan dipakai dalam implementasi printString() dan readString() dalam penulisan kernel.c. Setelah kernel.c dituliskan, maka kernel.c dicompile menggunakan bcc dan kernel.asm dicompile menggunakan nasm. Setelah keduanya dicompile, maka akan terbuat dua object code yang kemudian akan dilink menggunakan ld86.