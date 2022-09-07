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
    /*copia il pid*/
    int fd = open("./tmp/countpid", O_RDONLY);
    char pid[100];
    read(fd, pid, 100);
    if (fork() == 0)
    {
        /*esegue il comando*/
        execvp(argv[1], argv + 1);
    }
    else
    {
        /*manda un segnale sigusr1, poi aspetta il termine del comando
         *e poi manda un segnale sigusr2
        */
        kill((pid_t) atoi(pid), SIGUSR1);
        int status;
        wait(&status);
        kill((pid_t) atoi(pid), SIGUSR2);
    }
    
    
    return 0;
}
