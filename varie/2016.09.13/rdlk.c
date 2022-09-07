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
#include <sys/time.h>
#include <sys/eventfd.h>
#include <sys/inotify.h>
#include <sys/signalfd.h>
#include <sys/sendfile.h>

int main(int argc, char *argv[])
{
    char *fileR = argv[1];
    off_t off = 0;
    int fdR;
    struct flock lock;
    fdR = open(fileR, O_RDONLY);
    memset(&lock, 0, sizeof(lock));
    /*indica che si vuole bloccare in lettura*/
    lock.l_type = F_RDLCK;
    /*blocca il file*/
    fcntl(fdR, F_SETLKW);
    printf("Locked\n");
    sleep(rand() % 5);
    struct stat info;
    fstat(fdR, &info);
    /*Legge*/
    char buffer[info.st_size + 2];
    read(fdR, buffer, info.st_size);
    buffer[info.st_size] = '\n';
    buffer[info.st_size + 1] = '\0';
    write(STDOUT_FILENO, buffer, info.st_size + 2);
    /*indica che si vuole sbloccare*/
    lock.l_type = F_UNLCK;
    fcntl (fdR, F_SETLKW, &lock);
    sleep(rand() % 5);
    printf("Unocked\n");    
    close(fdR);
    return 0;
}