/* https://stackoverflow.com/questions/11042218/c-restore-stdout-to-terminal */
/* Sia dato il seguente programma: */
/* #include <stdio.h> */
/* #include <time.h> */
/* void redirectsetup(char *s) */
/* { */
/* } */
/* int main(int argc, char *argv[]) { */
/*  redirectsetup(argv[1]); */
/*  while (1) { */
/*  time_t now = time(NULL); */
/*  printf("%s",ctime(&now)); */
/*  sleep(1); */
/*  } */
/* } */
/* Senza modificare la funzione main, fare in modo che ricevendo un segnale SIGUSR1 l'output venga ridirezionato e */
/* aggiunto in append mode al file indicato come primo p */
#include <linux/limits.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
#include <stdlib.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define TRUE 1
#define FALSE 0
#include <fcntl.h>
#include <signal.h>
#include <limits.h>
char path[PATH_MAX];
void handler(int signo){
    static int boolean = FALSE;
    static int fd;
    static int STDOUT_FILENO_cpy;
    if (!boolean){
        boolean = TRUE;
        /* https://man7.org/linux/man-pages/man2/open.2.html */
        /* The argument flags must include one of the following access */
       /* modes: O_RDONLY, O_WRONLY, or O_RDWR.  These request opening the */
       /* file read-only, write-only, or read/write, respectively. */
        fd = open(path, O_WRONLY | O_APPEND);
        if (fd == -1) HANDLE_ERROR("open");
        STDOUT_FILENO_cpy = dup(STDOUT_FILENO);
        if (dup2(fd, STDOUT_FILENO) == -1) HANDLE_ERROR("dup2");
        close(fd);
    }
    else if (boolean){
        boolean = FALSE;
        if (dup2(STDOUT_FILENO_cpy, STDOUT_FILENO) == -1) HANDLE_ERROR("dup2()1");
        close(STDOUT_FILENO_cpy);
    }
}
void redirectsetup(char *s)
{
    printf("my pid: %d\n", getpid());
    struct sigaction sa; bzero(&sa, sizeof(struct sigaction));
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGUSR1, &sa, NULL) != 0) HANDLE_ERROR("sigaction");
    INIT_ARRAY(path);
    strncpy(path, s, PATH_MAX);
}
int main(int argc, char *argv[]) {
    redirectsetup(argv[1]);
    while (1) {
        time_t now = time(NULL);
        printf("%s",ctime(&now));
        sleep(1);
    }
}
