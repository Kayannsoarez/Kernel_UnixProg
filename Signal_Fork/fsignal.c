#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

const char * filename = "teste.x";

void handler(pid_t);

int main(void) {
  pid_t process_id;
  pid_t sid = 0;

  if ((process_id = fork()) < 0) {
    perror("Fork");
    exit(1);
  }

  if (process_id > 0) {
    signal(SIGUSR1, handler);
    for (;;)
      pause();

  } else {
    while (1) {
      if (access(filename, F_OK) == 0) {
        // Arquivo existe
        kill(0, SIGUSR1);
      } else {
        // Arquivo não existe	
      }
    }
  }

  // Altera as permissões de acesso, Zero significa que as permissões serão herdadas 
  umask(0);

  // Cria uma nova sessão
  sid = setsid();
  if (sid < 0) {
    exit(1);
  }

  // Fecha a entrada padrão, a saída padrão e a saída de erro padrão
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  exit(0);
}

void handler(int sig) {
  signal(SIGUSR1, handler);
  psignal(sig, "Sinal Recebido");
  printf("OK\n");
}
