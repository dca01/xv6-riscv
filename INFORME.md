Informe Tarea 1 - Sistemas Operativos 

Daniel Cortes Anjel dc_t1 29/9/25

Entorno: Windows 11 + WSL (Ubuntu 24.04), QEMU, toolchain RISC-V

## 1. Alcance
Se añadieron dos llamadas al sistema y se validaron con programas en espacio de usuario:
- `getppid()` — devuelve el PID del proceso padre; si no hay padre, retorna `-1`.
- `getancestor(int n)` — devuelve el PID del ancestro n del proceso actual. Para n=0 se obtiene el propio PID; si el ancestro solicitado no existe o n<0, retorna `-1`.

---

## 2. Implementacion
- `getppid()`: se consulta `myproc()->parent->pid`. Si `parent` es nulo, se retorna `-1`.
- `getancestor(n)`: se extrae `n` con `argint(0,&n)`, se valida `n>=0` y se asciende por `parent` n veces. Si durante el ascenso el puntero queda en nulo, se retorna `-1`; en caso contrario se retorna `p->pid`.

---

## 3. Cambios en el código
- `kernel/syscall.h`  
  `#define SYS_getppid 22`  
  `#define SYS_getancestor 23`
- `kernel/sysproc.c`  
  Implementaciones `sys_getppid()` y `sys_getancestor()` con la lógica anterior.
- `kernel/syscall.c`  
  Declaraciones `extern` y registros en la tabla `syscalls[]` para los IDs 22 y 23.
- `user/user.h`  
  Prototipos `int getppid(void);` y `int getancestor(int n);`
- `user/usys.pl`  
  Entradas `entry("getppid");` y `entry("getancestor");`
- `user/ppid.c`  
  Prueba mínima de `getppid()`.
- `user/yosoytupadre.c`  
  Pruebas de `getppid()` y `getancestor(n)` incluyendo creación de hijo con `fork()`.
- `Makefile`  
  Inclusión en `UPROGS` de `_ppid` y `_yosoytupadre`.

---

## 4. Ejecución y resultados

### Confirmacion
sh
make clean && make qemu
$ ppid
PID=3 PPID=2

$ yosoytupadre
[padre] PID=3, PPID=2
[padre] ancestor(0)=3
[padre] ancestor(1)=2
[padre] ancestor(2)=1
[hijo]  PID=4, PPID=3
[hijo]  ancestor(0)=4
[hijo]  ancestor(1)=3
[hijo]  ancestor(2)=2
[hijo]  ancestor(10)=-1

---

## 5. Conclusión

-No se presentaron mayores problemas en la instalacion.

-Se implementaron `getppid()` y `getancestor(n)` en xv6 y se probaron con `ppid` y `yosoytupadre` (incluidos en `UPROGS`). El comportamiento coincide con lo solicitado, incluyendo el retorno `-1` cuando no existe el ancestro. Además, se comprende el flujo de una syscall en xv6 (user.h/usys.pl → syscall.h/syscall.c → sysproc.c) y la relación padre–hijo.

