/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <signal.h> */
/* #include <unistd.h> */

/* int main( void ) */
/*   { */
/*     sigset_t set, oset, pset; */


/*     sigemptyset( &set ); */
/*     sigemptyset(&pset); */
/*     sigaddset( &set, SIGINT ); */
/*     sigaddset (&set, SIGUSR1); */
/*     sigprocmask( SIG_BLOCK, &set, &oset ); */
/*     /1* printf( "Old set was %8.8ld.\n", oset ); *1/ */

/*     /1* sigpending( &pset ); *1/ */
/*     printf( "Pending set is %8.8ld.\n", pset ); */

/*     /1* kill( getpid(), SIGINT ); *1/ */
/*     printf("my pid is %d. Press ENTER after you sent me a SIGINT\n", (int)getpid()); */
/*     getchar(); */

/*     sigpending( &pset ); */
/*     printf( "Pending set is %8.8ld.\n", pset ); */

/*     sigprocmask( SIG_UNBLOCK, &set, &oset ); */

/*     /1* The program terminates with a SIGINT *1/ */
/*     /1* return( EXIT_SUCCESS ); *1/ */
/*     while (1) { */
/*     } */
/*   } */










/* CELEBS22

   This example returns blocked or pending signals.

 */






/* https://www.ibm.com/docs/en/zos/2.2.0?topic=functions-sigpending-examine-pending-signals */
#define _POSIX_SOURCE
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void catcher(int signum) {
  puts("inside catcher!");
}

void check_pending(int signum, char *signame) {
  sigset_t sigset;

  if (sigpending(&sigset) != 0)
    perror("sigpending() error");
  else if (sigismember(&sigset, signum))
         printf("a %s signal is pending\n", signame);
       else
         printf("no %s signals are pending\n", signame);
}

main() {
  struct sigaction sigact;
  sigset_t sigset;

  sigemptyset(&sigact.sa_mask);
  sigact.sa_flags = 0;
  sigact.sa_handler = catcher;
  if (sigaction(SIGUSR1, &sigact, NULL) != 0)
    perror("sigaction() error");
  else {
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    if (sigprocmask(SIG_SETMASK, &sigset, NULL) != 0)
      perror("sigprocmask() error");
    else {
      puts("SIGUSR1 signals are now blocked");
      kill(getpid(), SIGUSR1);
      printf("after kill: ");
      check_pending(SIGUSR1, "SIGUSR1");
      sigemptyset(&sigset);
      sigprocmask(SIG_SETMASK, &sigset, NULL);
      puts("SIGUSR1 signals are no longer blocked");
      check_pending(SIGUSR1, "SIGUSR1");
    }
  }
}
