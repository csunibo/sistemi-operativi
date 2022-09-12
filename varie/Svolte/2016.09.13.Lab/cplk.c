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
    char *fileD = argv[2];
    char *fileS = argv[1];
    off_t off = 0;
    int fdD, fdS;
    struct flock lock;
    fdD = open(fileD, O_WRONLY);
    fdS = open(fileS, O_RDONLY);
    struct stat info;
    fstat(fdS, &info);
    memset(&lock, 0, sizeof(lock));
    /*indica che si vuole bloccare in scrittura*/
    lock.l_type = F_WRLCK;
    /*blocca il file*/
    fcntl(fdD, F_SETLKW);
    printf("Locked\n");
    sleep(rand() % 5);
    /*invia da fdS ad fdD da offset 0*/
    sendfile(fdD, fdS, &off, info.st_size);
    /*indica che si vuole sbloccare*/
    lock.l_type = F_UNLCK;
    fcntl (fdD, F_SETLKW, &lock);
    sleep(rand() % 5);
    printf("Unlocked\n");
    close (fdD);
    close (fdS);
    return 0;
}
