#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("non abbastanza argomenti\n");
    exit(1);
  }

  pid_t pid;
  int wstatus;
  int ret_val = 1;
  char *argo[argc];
  argo[argc - 1] = NULL;
  for (int i = 0; i < argc - 1; i++) {

    argo[i] = malloc(sizeof(char) * strlen(argv[i + 1]));
    strcpy(argo[i], argv[i + 1]);
  }

  do {
    switch (pid = fork()) {
    case -1:
      exit(1);
      break;
    case 0:
      printf("Esecuzione comando da processo figlio con pid:%d \n", getpid());
      execvp(argo[0], argo);
      break;
    default:
      if (waitpid(pid, &wstatus, 0) == -1)
        exit(1);
      ret_val = WEXITSTATUS(wstatus);
      break;
    }
  } while (ret_val == 0 && !(WIFSIGNALED(wstatus)));

  printf("Figlio ucciso o terminato con errore, addio\n");
  for (int i = 0; i < argc - 1; i++) {
    free(argo[i]);
  }

  return 0;
}
