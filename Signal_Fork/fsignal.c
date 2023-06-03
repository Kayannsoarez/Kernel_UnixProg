#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

const char *filename = "teste.x";

void handler(int);

int main(void) {
  pid_t process_id;
  pid_t sid = 0;

  if ((process_id = fork()) < 0) {
    // Exibe mensagem de erro se o fork falhar
    perror("Fork");
    exit(1);
  }

  if (process_id > 0) {
    // Processo pai
    for (;;) {
      // Aguarda a chegada de um sinal
      pause();
    }

  } else {
    // Processo filho

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

    // Configura o manipulador de sinal
    signal(SIGUSR1, handler);

    while (1) {
      if (access(filename, F_OK) == 0) {
        // Arquivo existe

        // Envia um sinal para o processo pai
        kill(getppid(), SIGUSR1);
        break;
      }
      // Aguarda por 1 segundo
      sleep(1);
    }

    exit(0);
  }
}

void handler(int sig) {
  // Configura o manipulador de sinal
  signal(SIGUSR1, handler);
  // Exibe a mensagem "OK" quando o sinal é recebido
  printf("OK\n");
}
