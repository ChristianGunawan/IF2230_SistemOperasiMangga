# Tugas Besar - Sistem Operasi - IF2230
**Kelompok Mangga K04**

## Anggota:
> * Gregorius Dimas Baskara 13519190
> * Christian Gunawan 13519199
> * Tanur Rizaldi Rahardjo 13519214

## Instruksi menjalankan
1. `chmod +x build-run.sh get-tools.sh run.sh`
2. Jika menjalankan untuk pertama kali, pastikan semua tools ada atau jalankan `./get-tools.sh`
3. Jika fscreate belum ada di folder other, jalankan `make filesystemcreator`
4. Jalankan command `./build-run.sh` untuk mengcompile & link seluruh file yang dibutuhkan dan akan menjalankan Bochs
5. Jika hanya ingin menjalankan gunakan `./run.sh`

## Program loadFile
Untuk program loadFile, gunakan `make fileloader` untuk membuat program, program akan terletak pada
folder other dengan nama **loadFile**.

Program menerima argumen pertama sebagai direktori target dan argumen kedua sebagai input file.
Status load file akan ditulis pada akhir program.

## Bonus
Semua bonus berhasil untuk diimplementasikan dalam milestone ini, meliputi:
1. Symbolic link (ln) dalam mode soft link dengan menggunakan flag "-s"
2. Autocomplete file yang tersedia untuk cd, ls, dan cat
3. Autocomplete folder saat melakukan cd
4. Shell memiliki history (4 command)

## Reference
1. Silberschatz, Galvin, Gagne. "Operating System Concepts", Chapter 10.
2. Asisten Sistem Terdistribusi. "Milestone 2 - 2021".
3. stanislavs.org/helppc/int_13.html

## Note
Milestone I dan II juga menggunakan beberapa fungsi pendukung yang terdapat di beberapa file selain kernel.c seperti:
1. opr.c
2. output.c
3. screen.c
4. shell.c
5. std.c
dan beberapa header yang juga menjadi header atas .c tersebut.

Pada Milestone III memiliki beberapa fitur:
cat

rm

mkdir

mv

cp

ln

## System Requirements
> * Ubuntu 18 or latest version

**TBA**
   
   
<!-- ALSA - Vcxsrv -
- Bochs troubleshoot,
chmod +x troubleshoot
Dependencies
ALSA - Vcxsrv - Ubuntu 20.04 - <TBA>
- xserver
https://medium.com/javarevisited/using-wsl-2-with-x-server-linux-on-windows-a372263533c3
https://www.stat.ipb.ac.id/agusms/index.php/2019/01/15/how-to-run-graphical-linux-applications-on-bash-on-ubuntu-on-windows-10/
- ALSA
https://bbs.archlinux.org/viewtopic.php?id=94696
objdump -D -b binary -m i8086 <filename>

https://www.fountainware.com/EXPL/bios_key_codes.htm
-->
