/* scrivere un programma para per lanciare parallelamente comandi */
/* para sleep 20 // ls -l // ps // sleep 3 */
/* deve eseguire concorrentemente i vari comandi separati da // e aspettare la terminazione di tutti */

/* estendere para a paran che ha un parametro che indica il numero massimo di esecuzioni concorrenti: */
/* paran 3 sleep 20 // ls -l // ps // sleep 3 */
/* al max esegue 3 comandi in parallelo, quindi esegue i primi 3 comandi, qundo uno fra essi termina */
/* ne attiva un successivo e cosi' via. */

//supponendo che gli argomenti passati vengano formattati correttamente

#include <stdio.h>
#include <stdlib.h>
#include "monitor.h"
#include <unistd.h>
#include "davolib.h"
#include <pthread.h>
#include <string.h>
#include <unistd.h>

monitor nbuff;
condition okToEnter;
int inBuf;
int n; 

void nbuff_create(){
    nbuff = monitor_create();
    okToEnter = condition_create(nbuff);
    inBuf = 0;
}

int execute(char *cmd){
    pid_t pid = fork();
    if (pid == -1){
        /* Failure */
        perror("fork");
        return -1;
    } else if (pid == 0){
        /* Child */
        int len = substring_occurrences(cmd, " ") +1;
        char *token[len + 1];//+1 perchè NULL TERMINATED
        split_string(cmd, " ", token);
        return execvp(token[0], token); //Control never returns to the original program unless there is an exec() error
    } else {
        /* Parent */
        wait(NULL); /* Wait for child */
        return 0;
    }
}

void *nbuff_enter(void *arg){
    usleep(random() % 200);
    monitor_enter(nbuff);
    printf("vuole entrare %s\n", (char *)arg);
    if (inBuf == n){
        printf("BLOCCATO %s\n", (char *)arg);
        condition_wait(okToEnter);
        printf("%s Sbloccato\n", (char *)arg);
    } else printf("%s subito entrato\n", (char *)arg);
    inBuf++;
    monitor_exit(nbuff);

    execute((char *)arg);

    monitor_enter(nbuff);
    inBuf--;
    condition_signal(okToEnter);
    monitor_exit(nbuff);
}


int main(int argc, char const *argv[]){

    n = atoi(argv[1]);
    #define MAX_THREADS 50
    pthread_t threads[MAX_THREADS];

    nbuff_create();

    srandom(time(NULL));

    int j = 0;
    for (int i=2; i<argc; i++){
        if (strcmp(argv[i], "//") == 0) continue;
        pthread_create(&threads[j], NULL, nbuff_enter, (void *)argv[i]); 
        j++;
    }
    for (int k=0; k < j; k++){
        pthread_join(threads[k], NULL); 
    }

}
