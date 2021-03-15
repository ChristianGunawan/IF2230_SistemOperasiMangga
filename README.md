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
<!-- 
# Tugas Besar 1 - Strategi Algoritma - IF2211
**TBA**



## Tentang Program
- Engine game membutuhkan dependencies yang dilist pada tabel dibawah.
- Game dapat dijalankan secara langsung dengan menjalankan *run.bat* pada OS Windows.
- Tugas besar ini juga tersedia pada [repository github](https://github.com/Lock1/Stima1-IF2211).
- Direkomendasikan untuk menggunakan visualizer ketika ingin melihat match yang telah dijalankan pada program utama.



**Dependencies**
Komponen  | Menggunakan
--------  | -----------
JDK / JRE | 8
NodeJS    | 15.0.1


**Anggota Kelompok**
NIM      | Nama
---      | --------------------------
13519140 | Fabian Savero Diaz Pranoto
13519146 | Fadel Ananda Dotty
13519214 | Tanur Rizaldi Rahardjo




<!--
### Release
[v1.0](https://github.com/Lock1/Logif-IF2121/releases/tag/v1.0)


### Tentang Program
- Release versi **1.0** menandai bahwa release tersebut adalah **program yang dikumpulkan** ketika deadline, tanpa perubahan.
- **Tidak disarankan untuk menjalankan program menggunakan terminal gprolog pada windows.**
- Program ini menggunakan fitur pergerakan kursor & coloring ANSI dan Unicode box-drawing character yang hanya dapat ditampilkan pada terminal yang mensupportnya.
- Jika menggunakan terminal yang tidak mensupportnya, **sangat disarankan** untuk mendisable semua UI yang menggunakan kedua fitur diatas.
- Program diuji dan didevelop dengan **WSL**.
- Pastikan Distro menggunakan GLIBC versi **2.29** keatas (Untuk Ubuntu, 20.04+).
- Jalankan linux executable play untuk mencoba permainan, **good luck have fun!**

### Style kode
- Program ini menggunakan camel-case, dikarenakan tidak mengetahui prolog menggunakan snake-case.
- Kode blok **or** yang besar, digunakan indentasi dan bracket untuk memperjelas kode.


<!-- put some random isekai'd human and amulet of yendor here


## Snapshot
**Now Loading** \
![UI](/other/img/loading.gif) \

**UI & Movement** \
![UI](/other/img/movement.gif) \

**Battle** \
![UI](/other/img/battle.gif) \

**I think I see it somewhere...** :thinking: \
![UI](/other/img/portal.gif)



## Informasi Dasar
Data                 | Isi
----                 | ---
Tahun Ajaran         | 2021
Tanggal Mulai        | 9 November 2020
Tanggal Selesai      | 25 November 2020
Tanggal Pengumpulan  | 27 November 2020
Kelas                | 2
Dosen                | Fariska Zakhralativa Ruskanda
Asisten              | TBA
Nama Kelompok        | send help
Kelompok             | 6


**Anggota Kelompok**
NIM      | Nama
---      | ------------------
13519146 | Fadel Ananda Dotty
13519170 | La Ode Rajuh Emoko
13519178 | Akeyla Pradia Naufal
13519214 | Tanur Rizaldi Rahardjo


**Tools yang digunakan**
Komponen                      | Menggunakan
--------                      | -----------
Windows Subsystem for Linux   | 2
Ubuntu                        | 20.04
GNU/Prolog                    | 1.4.5
Ascgen2                       | 2.0.0


**Dependencies**
Komponen | Menggunakan
-------- | -----------
GLIBC    | 2.29

Program ini tidak membutuhkan library eksternal \
Seluruh program dikoding menggunakan
fitur standard GNU/Prolog.


## Spesifikasi
**Status : Done** :green_square: \
**10 / 10**
Spesifikasi               | Status
-----------               | ------
Character & Class system  | :green_square:
Map & Draw procedure      | :green_square:
Inventory                 | :green_square:
Items                     | :green_square:
Enemy                     | :green_square:
Quest                     | :green_square:
Exploration Mechanism     | :green_square:
Battle Mechanism          | :green_square:
Store                     | :green_square:
Goal & Fail state         | :green_square:


**Bonus** \
**Status : Done** :green_square:
Spesifikasi   | Status
-----------   | ------
Effect Potion | :green_square:
Monster Zone  | :green_square:
Teleport      | :green_square:
Save & Load   | :green_square:



**Extras**
Spesifikasi           | Status
-----------           | ------
Raw movement mode     | :green_square:
Basic map UI          | :green_square:
Critical & Dodge Stat | :green_square:
Balancing             | :green_square:
Floor system          | :green_square:
Equipment UI          | :green_square:


## Laporan
**Status : Done** :green_square: \
**3 / 3**
Bab                   | Status
---                   | ------
Penjelasan Command    | :green_square:
Skenario Penggunaan   | :green_square:
Snapshot              | :green_square:


### Fun Fact
- Program ini juga berisi cheat dari beberapa game yang cukup terkenal.
- Banyak hal diprogram ini dibuat dengan referensi game ataupun anime, cobalah temukan referensi tersebut :)
###### Ya, game ini merupakan direct reference untuk rogue. (Dengan mix-max spesifikasi tentunya)

-->
 -->

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
