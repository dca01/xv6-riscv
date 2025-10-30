Informe Tarea 2 - Sistemas Operativos 

Daniel Cortes Anjel grupoM_t2 30/10/25

Entorno: Windows 11 + WSL (Ubuntu 24.04), QEMU, toolchain RISC-V

## 1. Funcionamiento y lógica de la implementación
Se reemplazó el Round-Robin por Lottery Scheduling. Cada proceso mantiene un campo tickets (>=1) y un contador cpu_slices (veces elegido). En cada iteración del scheduler():

Se recorre la tabla de procesos y se calcula total=tickets de todos los RUNNABLE (forzando tickets>=1 por robustez).

-Se genera r en [1..total] mediante un LCG sencillo (lcg_rand).

-Se recorre nuevamente acumulando tickets hasta que acc >= r; ese proceso es el winner.

-Con el lock del ganador tomado, se marca RUNNING, se hace cpu_slices++ y se ejecuta con swtch.

-Si total==0, se deshabilitan interrupciones y se ejecuta wfi para dormir hasta la siguiente interrupción, luego continúa el bucle.

Evidencia (demo): programa user/demo.c crea N=10 hijos con settickets(50*(i+1)). El kernel imprime en kexit() las métricas pid/tickets/slices. Se observa la tendencia tickets slices (proporcionalidad en promedio).

## 2.Explicación de las modificaciones realizadas (archivos y cambios clave)

- kernel/proc.h :

int tickets; (por defecto 100, mínimo 1)

int cpu_slices; (conteo de elecciones)

uint64 ctime; uint64 etime; (marcas de creación/termino)

- kernel/proc.c :

allocproc(): inicializa tickets=100, cpu_slices=0, ctime=ticks, etime=0.

kexit(): asigna etime=ticks y (para pruebas) imprime:
printf("KERNEL: pid=%d tickets=%d slices=%d\n", ...).

scheduler(): lógica de lotería: sumatoria → sorteo → acumulado → ganador (con cpu_slices++).

RNG simple: static uint lcg_rand(void) y semilla estática.

- Syscall settickets(int n) :

kernel/syscall.h: #define SYS_settickets <ID>

kernel/syscall.c: extern uint64 sys_settickets(void); y entrada en syscalls[].

kernel/sysproc.c: implementación sys_settickets: lee n con argint(0,&n), si n<1; n=1, y asigna a myproc()->tickets.

user/user.h: prototipo int settickets(int n);

user/usys.pl: entry("settickets");

- user/demo.c :

Crea 10 procesos, llama settickets(50*(i+1)), ejecuta carga CPU, el padre espera a todos y reporta termino

- Makefile :

Agregado _demo a UPROGS

## 3.Dificultades encontradas y soluciones implementadas

No se presentaron mayores problemas en el desarollo de la tarea.

## 4.Posibles problemas del Lottery Scheduling

Varianza estocástica: en ventanas pequeñas, un proceso con menos tickets podría ser elegido varias veces seguidas por azar.

Sin garantías duras de latencia/plazos: el esquema es probabilístico; no asegura deadlines. Para cargas de tiempo real, requiere complementos (p. ej., prioridades o cuotas).

Dominancia por tickets altos: procesos con muchos tickets pueden acaparar CPU; si no hay política institucional para otorgar tickets, podria introducir sesgos.

No distingue I/O-bound vs CPU-bound: sin mecanismos adicionales, un CPU-bound con más tickets puede afectar la responsividad de I/O-bound.

Semilla/PRNG simples: un generador pobre puede degradar la distribución

Tuning operacional: valores por defecto (tickets=100) funcionan, pero distintas cargas pueden requerir ajustes o aging para evitar inanición.

## 5. Ejecución y resultados

make clean && make qemu
$ demo
demo: creando 10 hijos con distintos tickets (50..500)
KERNEL: pid=12 tickets=450 slices=4
[padre] hijo pid=12KERNEL: pid=9 tickets=300 slices=4
KERNEL: pid=10 tickets=350 slices=5
 termino (status=0)
[padre] hijo pid=9 termino (statKERNEL: pid=13 tickets=500 slices=5
KERNEL: pid=7 tickets=200 slices=4
KERNEL: pid=8 tickets=250 slices=6
us=0)
[padre] hijo pid=7 termino (status=0)
[padre] hijo pid=8 termino (status=0)
[padre] hijo pid=10 termino (status=0)
[padre] hijo pid=13 termino (status=0)
KERNEL: pid=5 tickets=100 slices=6
[padre] hijo pid=5 termino (status=0)
KERNEL: pid=4 tickets=50 slices=4
[KERNEL: pid=6 tickets=150 slices=5
padre] hijo pid=4 termino (status=0KERNEL): pid=11 tickets=400 slices=5

[padre] hijo pid=6 termino (status=0)
[padre] hijo pid=11 termino (status=0)
demo: fin
KERNEL: pid=3 tickets=100 slices=15
