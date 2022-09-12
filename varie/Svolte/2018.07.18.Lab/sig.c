#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> 
#include <stdlib.h>
#include <errno.h>  
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>
#include <string.h>
#include <grp.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <sys/prctl.h>
#include <pthread.h>
#include <time.h>
#include <sys/eventfd.h>
#include <sys/inotify.h>
#include <sys/signalfd.h>

int main(int argc, char const *argv[])
{
    /*in ordine:
      creiamo un signal set
      ci aggiungiamo i segnali che vogliamo ricevere
      cambiamo la maschera dei segnali del processo con quella impostat
      apriamo un signal fd
    */
    int val = 0;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    int sfd = signalfd(-1, &mask, 0);
    printf("My pid is %d\n", getpid());

    for(;;)
    {
        struct signalfd_siginfo fdsi;
        read(sfd, &fdsi, sizeof(fdsi));
        if (fdsi.ssi_signo == SIGUSR1 || fdsi.ssi_signo == SIGUSR2)
        {
            if (fdsi.ssi_signo == SIGUSR1)
            {
                val++;
            }
            else
            {
                val--;
            }
            printf("Val: %d\n", val);    
        }
        
    }
    return 0;
}
