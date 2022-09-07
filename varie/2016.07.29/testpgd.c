/* scrivere la funzione: */
/* void printgroupdir(const char *dirp, const char *group); */
/* che deve scandire la directory con pathname dirp e stampare tutti I file (non le directory) che appartengano al */
/* gruppo passato come secondo parametro. */
/* Creare I file printgroupdir.c, printgroupdir.h e un file testpgd.c contenente un programma principale che prenda */
/* come argv[1] e argv[2] i due parametri della funzione. Produrre un Makefile opportuno. */
/* Esempi per provare il funzionamento possono essere: */
/* testpgd /dev tty */
/* testpgd /dev disk */
#include <bits/types/sigset_t.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <signal.h>
#include "printgroupdir.h"
void sigsev_handler(int signo){
    /* if (signo == SIGSEGV){ */
        printf("\nONE SIGSEGV was expected. We're gonna to disable this handler");
    /* } */
}
int main(int argc, char **argv){
    if (argc != 3){
        fprintf(stderr, "\nBAD USAGE");
        exit(EXIT_SUCCESS);
    }
    //imposto l'handler e blocco le gestioni del segnale predefinite
    struct sigaction sact; bzero(&sact, sizeof(struct sigaction));
    sigemptyset(&(sact.sa_mask)); //init
    sigaddset(&(sact.sa_mask), SIGSEGV); //vs sigfillset
    sact.sa_flags = SA_RESTART;
    sact.sa_handler = sigsev_handler;
    sigaction(SIGSEGV, &sact, NULL);
    /* sigset_t ns; */ //ALTRIMENTI PENDENTE SIGNAL
    /* sigemptyset(&ns); */
    /* sigaddset(&ns, SIGSEGV); */
    /* sigprocmask(SIG_BLOCK, &ns, NULL); */

    char **res = groupdir(argv[1], argv[2]);
    /* https://stackoverflow.com/questions/6812242/defining-and-iterating-through-array-of-strings-in-c */
    char **tmp = res;
    for (int i = 0; i< MAX_FILES; i++){
        if (res[i] == NULL) {
            printf("\nINCONTRATO1!");
            break;
        }
        else printf("%s\n", res[i]);
    }
    /* while (*tmp != (char *)0){ */
    /*     printf("\n%s", *tmp++); */
    /* } */
    freestringarray(res);

    for (int i = 0; i< MAX_FILES; i++){
        if (res[i] == (char *)0){
            printf("\nINCONTRATO!");
            /* break; */
        }
        else printf("%s\n", res[i]);
        /* sigprocmask(SIG_SETMASK, &ns,NULL); */
    }
    kill(getpid(), SIGSEGV);
    /* while (*res != (char *)0){ */
    /*     printf("\n%s", *res++); */
    /* sigprocmask(SIG_SETMASK, &ns,NULL); */
    /* } */

    exit(EXIT_SUCCESS);
}
