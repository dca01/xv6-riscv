#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  char *addr = sbrk(0);
  if(sbrk(4096) < 0){
    printf("sbrk failed\n");
    exit(1);
  }

  addr[0] = 'A'; //escritura antes de proteger

  if (mrdprotect(addr, 1) < 0) {
    printf("mrdprotect falló\n");
    exit(1);
  }

  int pid = fork();
  if(pid == 0){
    volatile char c = addr[0];//si se lee muere
    printf("Valor leído: %c (NO debería imprimirse)\n", c);
    exit(0);
  } else {
    int st;
    wait(&st);
    printf("Hijo terminó con status=%d (esperado: killed)\n", st);

    if (munrdprotect(addr, 1) < 0) {
      printf("munrdprotect falló\n");
      exit(1);
    }

    printf("Lectura tras unprotect: %c (esperado A)\n", addr[0]);
  }

  exit(0);
}
