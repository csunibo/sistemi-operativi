/* Scrivere un programma catsig che copi lo standard input nello standard output (come il comando cat senza */
/* parametri, per intenderci) e che stampi la scritta “ho ricevuto un segnale” quando riceve un segnale SIGUSR1. */
/* La particolarita' di questo programma e' che per la gestione dei segnali deve usare la chiamata di sistema */
/* signalfd (e non la signal o la sigaction) */
//thread o fork, riguardo cat senza parametri usa fgets https://linuxconfig.org/cat-command#:~:text=When%20the%20cat%20command%20does,by%20CTRL%2BD%20key%20combination.
#include <stdio.h>
#include <sys/poll.h>
#include <unistd.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <stdlib.h>
#include <strings.h>
#include <poll.h>
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
#include <stdlib.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define TRUE 1
#define FALSE 0
int main(){
    char *scritta = "ho ricevuto un segnale";
    printf("my pid: %d\n", getpid());
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) HANDLE_ERROR("sigprocmask");
    int fd = signalfd(-1, &mask, 0);
    if (fd == -1) HANDLE_ERROR("signalfd");


    while (1);
    while (TRUE){
        if (poll(fds, 1, -1) == 0) HANDLE_ERROR("poll");
        if (fds[0].revents & POLLIN) {
            struct signalfd_siginfo fdsi;
            if (read(fds[0].fd, &fdsi, sizeof(fdsi)) != sizeof(fdsi)) HANDLE_ERROR("read");
            if (fdsi.ssi_signo == SIGUSR1){
                printf("%s\n", scritta);
            }
        }
    }
    exit(EXIT_SUCCESS);
}
