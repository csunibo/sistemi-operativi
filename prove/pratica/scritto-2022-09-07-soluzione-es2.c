#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ONE_S_IN_US 1000000

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("non abbastanza argomenti\n");
    exit(1);
  }
  struct timeval t1, t2;
  long int usdiff;

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
    gettimeofday(&t1, NULL);
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
      gettimeofday(&t2, NULL);
      usdiff =
          ((t2.tv_sec - t1.tv_sec) * ONE_S_IN_US) + (t2.tv_usec - t1.tv_usec);
      printf("Durata: %ldus\n", usdiff);
      break;
    }
  } while (ret_val == 0 && !(WIFSIGNALED(wstatus)) && (usdiff >= ONE_S_IN_US));

  if (usdiff < ONE_S_IN_US)
    printf("Figlio troppo veloce, addio\n");

  if ((ret_val != 0) || WIFSIGNALED(wstatus))
    printf("Figlio ucciso o terminato con errore, addio\n");

  for (int i = 0; i < argc - 1; i++) {
    free(argo[i]);
  }

  return 0;
}
