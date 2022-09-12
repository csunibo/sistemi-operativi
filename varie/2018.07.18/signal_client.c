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
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char const *argv[])
{
    if (argc < 2){
        fprintf(stderr, "\nBAD USAGE");
        exit(EXIT_FAILURE);
    }
    FILE *stream = fopen("/tmp/countpid", "r");
    if (stream == NULL) HANDLE_ERROR("\nfopen()");

    int itsPid = -1;
    if (fread(&itsPid, sizeof(int), 1, stream) == 0){
        HANDLE_ERROR("\nfread()");
    }
    fclose(stream);
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
        kill((pid_t) itsPid, SIGUSR1);
        int status;
        wait(&status);
        kill((pid_t) itsPid, SIGUSR2);
    }


    return 0;
}
