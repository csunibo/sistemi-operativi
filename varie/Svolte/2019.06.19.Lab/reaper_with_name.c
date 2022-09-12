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

int main(int argc, char const *argv[])
{
    /*il PR_SET_CHILD_SUBREAPER non viene ereditato dalla fork*/
    prctl(PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0, 0);

    prctl(PR_SET_NAME, "Nonno", 0, 0, 0, 0);
    /*creazione primo figlio*/
    if (fork() == 0)
    {
        /*creazione nipote*/
        if (fork() != 0)
        {
            prctl(PR_SET_NAME, "Figlio", 0, 0, 0, 0);
            sleep(1);
            printf("First child ends, pid: %d, parent: %d\n", getpid(), getppid());
            exit(0);
        }
        else
        {
            prctl(PR_SET_NAME, "Nipote", 0, 0, 0, 0);
            printf("Granchild starts, pid: %d, parent: %d\n", getpid(), getppid());
            sleep(2);
            printf("Granchild ends, pid: %d, parent: %d\n", getpid(), getppid());
            exit(0);
        }
    }
    
    /*il padre aspetta entrambi*/
    else
    {
        for (int i = 0; i < 2; i++)
        {
            int status;
            pid_t pid = wait(&status);

            printf("wait for pid: %d\n", pid);
        }
        
    }
    
    return 0;
}
