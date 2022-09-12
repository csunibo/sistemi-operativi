/* https://stackoverflow.com/questions/64763144/fork-a-process-with-multiple-thread-c */
/* https://stackoverflow.com/questions/4200516/is-there-a-way-to-run-any-exec-family-function-as-a-thread-in-c */
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
void *normalThread(){
    sleep(2);
    printf("\nnormal thread still running => non sovrascrive tutto!");
    return NULL;
}

void *specialThread(){
    pid_t pid = fork();
    if (pid == -1){
        perror("specialThread fork error");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        printf("\nfiglio specialThread running...");
        sleep(2);
    }
    else {
        execlp("ls", NULL);
    }
    return NULL;
}

int main(){
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, normalThread, NULL);
    pthread_create(&tid2, NULL, specialThread, NULL);
    sleep(3);
    printf("\nactually I'm a thread as well!");
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}
//risultato = exec sovrascrive tutto :(
