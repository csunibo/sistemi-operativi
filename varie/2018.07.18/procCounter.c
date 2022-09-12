#include <bits/types/sigset_t.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signalfd.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
//ricordati di passargli il puntatore!
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )

void HANDLE_ERROR(const char *msg){
    do { perror(msg); exit(EXIT_FAILURE); } while (0);
}
int main(int argc, char **argv){
    int myPid = (int) getpid();
    fprintf(stderr, "\nMy pid is %d", myPid);
    /* int fd_tmp = open("/tmp/countpid", O_CREAT | O_WRONLY, 0777); */
    /* if (fd_tmp == -1) HANDLE_ERROR("\nopen()"); */

    FILE *stream = fopen("/tmp/countpid", "w+");
    if (stream == NULL) {HANDLE_ERROR("\nfdopen()");}

    if (fwrite(&myPid, sizeof(int), 1, stream) == 0){
        HANDLE_ERROR("\nfwrite()");
    }
    fclose(stream); //IMP fprintf

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0){
        HANDLE_ERROR("\nsigprocmask()");
    }
    int fd = signalfd(-1, &mask, 0);
    if (fd == -1) HANDLE_ERROR("\nsignalfd()");

    int counter = 0; //contatore processi attivi

    while(1){
        struct signalfd_siginfo ret; bzero(&ret, sizeof(ret));

        int len = read(fd, &ret, sizeof(ret));
        if ( (len != sizeof(struct signalfd_siginfo)) || (len < 0) ){
            HANDLE_ERROR("\nread()");
        }
        if (ret.ssi_signo == SIGUSR1){
            counter++;
            printf("Un processo in più: ora i processi attivi sono %d\n", counter);
        }
        if (ret.ssi_signo == SIGUSR2){
            counter--;
            printf("Un processo in meno: ora i processi attivi sono %d\n", counter);
        }
    }


    exit(EXIT_SUCCESS);
}
