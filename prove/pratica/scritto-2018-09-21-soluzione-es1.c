#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signalfd.h>
#include <time.h>
#include <unistd.h>

#define DIM 10

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

void update(pid_t pid, int signal) {
  char filename[DIM];
  if (snprintf(filename, DIM, "%d", pid) < 0)
    handle_error("snprintf");
  FILE *f = fopen(filename, "a");
  printf("Hey: %s\n", filename);
  time_t t = time(NULL);
  fprintf(f, "%s %s",
          signal == SIGUSR1   ? "USR1"
          : signal == SIGUSR2 ? "USR2"
                              : "????",
          ctime(&t));
  fclose(f);
}

int main(int argc, char *argv[]) {
  printf("Send signals to me: %d\n", getpid());

  sigset_t mask;
  int sfd;
  struct signalfd_siginfo fdsi;
  ssize_t s;

  sigemptyset(&mask);
  sigaddset(&mask, SIGUSR1);
  sigaddset(&mask, SIGUSR2);

  /* Block signals so that they aren't handled
     according to their default dispositions */

  if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
    handle_error("sigprocmask");

  sfd = signalfd(-1, &mask, 0);
  if (sfd == -1)
    handle_error("signalfd");

  for (;;) {
    s = read(sfd, &fdsi, sizeof(fdsi));
    if (s != sizeof(fdsi))
      handle_error("read");

    if (fdsi.ssi_signo == SIGUSR1 || fdsi.ssi_signo == SIGUSR2)
      update(fdsi.ssi_pid, fdsi.ssi_signo);
  }
}
