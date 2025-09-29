#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int me = getpid();
  int p  = getppid();

  printf("[padre] PID=%d, PPID=%d\n", me, p);
  printf("[padre] ancestor(0)=%d (self)\n", getancestor(0));
  printf("[padre] ancestor(1)=%d (ppid)\n", getancestor(1));
  printf("[padre] ancestor(2)=%d (abuelo o -1)\n", getancestor(2));

  int pid = fork();
  if (pid == 0) {
    // Hijo
    int me_c = getpid();
    int p_c  = getppid();
    printf("[hijo]  PID=%d, PPID=%d\n", me_c, p_c);
    printf("[hijo]  ancestor(0)=%d\n", getancestor(0));
    printf("[hijo]  ancestor(1)=%d\n", getancestor(1));
    printf("[hijo]  ancestor(2)=%d\n", getancestor(2));
    printf("[hijo]  ancestor(10)=%d (espera -1)\n", getancestor(10));
    exit(0);
  } else if (pid > 0) {
    wait(0);
  } else {
    printf("fork fallo\n");
  }
  exit(0);
}
