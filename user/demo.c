#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define N 10
volatile int sink;

static void burn_cpu(int loops){
  for (volatile int i = 0; i < loops; i++) sink += i;
}

int
main(void){
  printf("demo: creando %d hijos con distintos tickets (50..500)\n", N);

  for (int i = 0; i < N; i++){
    int pid = fork();
    if (pid < 0){
      printf("fork fallo\n");
      exit(1);
    }
    if (pid == 0){
      int t = 50 * (i + 1);   // 50,100,...,500
      settickets(t);
      // carga de CPU suficiente para observar proporcionalidad
      burn_cpu(70 * 1000000);
      exit(0);
    }
    // padre: no necesitamos guardar pids
  }

  // esperar a que terminen los N hijos
  for (int i = 0; i < N; i++){
    int st;
    int cpid = wait(&st);
    printf("[padre] hijo pid=%d termino (status=%d)\n", cpid, st);
  }

  printf("demo: fin\n");
  exit(0);
}
