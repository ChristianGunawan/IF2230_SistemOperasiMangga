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

Resource
https://www.fountainware.com/EXPL/bios_key_codes.htm

Code Styling
Variable -> snake_case
Function -> camelCase
-->
