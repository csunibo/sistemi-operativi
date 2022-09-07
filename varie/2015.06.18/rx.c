/* Usando signalfd (e non signal o sigaction) scrivere due programmi tx e rx. */
/* Rx deve essere attivato per primo e per prima cosa stampa il suo pid e aspetta. */
/* Tx ha due parametri: il pid del ricevente rx e una stringa. Rx deve stampare la stringa passata come secondo */
/* parametro a Tx e uscire. La stringa deve essere spedita usando solo segnali. Ogni bit di ogni carattere deve essere */
/* inviato usando SIGUSR1 se vale 1 e SIGUSR2 se vale 0. Al ricevimento di ogni bit rx spedisce un segnale di */
/* acknowledgment a tx che provvede a spedire il successivo. */
#include <bits/types/sigset_t.h>
#include <stdio.h>
#include <sys/signalfd.h>
#include <unistd.h>
#include <signal.h>
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
#include <stdlib.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define TRUE 1
#define FALSE 0
#include <strings.h>
#include <string.h>
int main(){
    printf("my pid is %d\n", getpid());
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) HANDLE_ERROR("sigprocmask");
    int fd = signalfd(-1, &mask, 0);
    if (fd == -1) HANDLE_ERROR("signalfd");
    struct signalfd_siginfo fdsi;// bzero(&fdsi, sizeof(struct signalfd_siginfo));
    char bin[9]; INIT_ARRAY(bin); bin[8] = '\0';
    int i = 0;
    while (TRUE){
        if (read(fd, &fdsi, sizeof(struct signalfd_siginfo)) != sizeof(struct signalfd_siginfo)) HANDLE_ERROR("read");
        if(  fdsi.ssi_signo == SIGUSR1 ) {
            fprintf(stderr, "%d", 1);
            sprintf(&bin[i], "%d", 1);
        }
        /* fprintf(stderr, "%d", ); */
        else if(  fdsi.ssi_signo == SIGUSR2 ){
            fprintf(stderr, "%d", 0);
            sprintf(&bin[i], "%d", 0);
        }
        i++;
        kill(fdsi.ssi_pid, SIGUSR1); //ACK
        if (i == 8){ //no strlen perchè c'è già in fondo NULL quindi già considerata stringa (?)
            /* fprintf(stderr, "\n%s\n", bin); */
            //controlliamo che non sia il "carattere" di terminazione 00000000 (NULL)
            int boolean = TRUE;
            for (int j = 0;(   j <  8) && boolean; j++){
                if (bin[j] != '0'){
                    boolean = FALSE;
                }
            }
            if (boolean) exit(EXIT_SUCCESS);
            char c = strtol(bin, (char **)NULL, 2);
            fprintf(stderr, "%c", c);
            i = 0;
            c = 0x0;
            INIT_ARRAY(bin); bin[8] = '\0';
        }
    }
}
