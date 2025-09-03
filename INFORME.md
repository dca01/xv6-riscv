Informe Tarea 0 - Sistemas Operativos

Daniel Cortes Anjel
dc_t0
3/9/25

1)Primero instale WSL (se adjuntaba al final del enunciado de la T0 en Webc para trabajar en 
Windows 11). Luego instale Ubuntu para trabajar de manera estandar. Actualice el sistema para 
dejarlo todo listo.

2)Luego instale lo requerido para correr xv6. Git, build essential, qemu system misc y el compilador
gcc riscv64 linux gnu.

3)Luego clone el repositorio aca en linux con git clone https://github.com/mit-pdos/xv6-riscv.git 

4)Se crea la rama git checkout -b dc_t0

5)Compilar y ejecutar qemu con make qemu

-No se presentaron mayores problemas en la instalacion.

-Confirmacion xv65 funcionando:
$ ls 

.              1 1 1024
..             1 1 1024
README         2 2 2226
cat            2 3 32864
echo           2 4 31720
forktest       2 5 15856
grep           2 6 36240
init           2 7 32216
kill           2 8 31680
ln             2 9 31504
ls             2 10 34816
mkdir          2 11 31536
rm             2 12 31528
sh             2 13 54168
stressfs       2 14 32608
usertests      2 15 178800
grind          2 16 47528
wc             2 17 33816
zombie         2 18 31056

$ echo "Hola xv6"
Hola xv6

$ cat README
xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix
Version 6 (v6).  xv6 loosely follows the structure and style of v6,
but is implemented for a modern RISC-V multiprocessor using ANSI C.

La tarea se desarrollo en Windows 11, trabajando localmente con WSl donde se instalo
Ubuntu 24.04.3 LTS. QEMU y RISC-V quedaron listos para usar XV6.





