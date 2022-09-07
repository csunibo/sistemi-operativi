/* Lo scopo di questo esercizio è di scrivere un programma che usando la system call signalfd quando riceve un */
/* segnale SIGUSR1 o SIGUSR2 scriva in un file della directory corrente con nume uguale al pid del processo che ha */
/* inviato il segnale, il tipo di segnale ricevuto e l'ora esatta di ricezione del segnale. (se il file non esiste deve essere */
/* creato). */
/* un esempio di file risultante potrebbe essere: */
/* $ cat 20231 */
/* USR1 Fri Sep 21 10:40:56 CEST 2018 */
/* USR1 Fri Sep 21 10:40:58 CEST 2018 */
/* USR2 Fri Sep 21 10:41:26 CEST 2018 */
/* https://gist.github.com/madmax440/acf9be54ff0a4297382f */
/* to set time date https://stackoverflow.com/questions/2916170/system-time-setting-using-c-library-function */
//https://riptutorial.com/c/example/29223/read-lines-from-a-file => loop con array di tempo da aspettare
#include <bits/types/sigset_t.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/signalfd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
void uint32TOstr(uint32_t tmp){
	char converted[4];
	for(int i=0; i<4; i++){
		converted[i]=tmp%256;
		tmp/=256;
	}
    printf("prova conversione: %s\n", converted);
}
int main(){
    printf("my pid is %lu", (unsigned long) getpid());
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigaddset(&mask, SIGINT);
    if ( sigprocmask(SIG_BLOCK, &mask,NULL) < 0){
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }
    int fd = signalfd(-1, &mask, 0);
    if (fd == -1) {
        perror("signalfd");
        exit(EXIT_FAILURE);
    }
    while (1){
        struct signalfd_siginfo ret;
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        char code[8];
        memset(code, 0, sizeof(code));

        int l = read(fd, &ret, sizeof(struct signalfd_siginfo));
        if ((l != sizeof(struct signalfd_siginfo)) || (l < 0)) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        pid_t pid = (pid_t) ret.ssi_pid;
        /* https://cboard.cprogramming.com/c-programming/104485-how-convert-uint32_t-string.html */
        /* buf[0] = pid >> 24; */
        /* buf[1] = pid >> 16; */
        /* buf[2] = pid >> 8; */
        /* buf[3] = pid; */
        /* snprintf(buf, sizeof(buf), "ciao"); */
        /* buf[4] = '\0'; */
        uint32TOstr((uint32_t) 32);
        uint32TOstr((uint32_t) 42); //stampa asterisco correttamente, prova a passargli una stringa!
        time_t t = time(NULL);
        snprintf(buf, sizeof(buf), "%d %s ", pid, ctime(&t));

        printf("%lu, my pid: %lu\n", (unsigned long)pid, (unsigned long)getpid());
        printf("%s\n", buf);

        char nome[6];
        memset(nome, 0, sizeof(nome));
        snprintf(nome, sizeof(nome), "%d", pid);

        int fdPID = openat(AT_FDCWD, nome, O_CREAT | O_RDWR | O_APPEND, 0777);
        if (fdPID == -1){
            perror("fdPID");
            exit(EXIT_FAILURE);
        }
        FILE *stream = fdopen(fdPID, "a");
        if (stream == NULL) {
            perror("fdopen");
            exit(EXIT_FAILURE);
        }
        if (ret.ssi_signo == SIGUSR1){ //allora incremento il valore, altrimento lo diminuisco
            snprintf(code, sizeof(code), "SIGUSR1");
            /* code[strlen(code) -1] = '\0'; */
        }
        else if (ret.ssi_signo == SIGUSR2){
            snprintf(code, sizeof(code), "SIGUSR2");
            /* code[strlen(code) -1] = '\0'; */
        }
        else if (ret.ssi_signo == SIGINT){
            snprintf(code, sizeof(code), "SIGINT");
        }
        printf("code: %s\n", code);

        strcat(buf, code);
        strcat(buf, "\n");

        if ( fwrite(buf, sizeof(char), strlen(buf), stream) != strlen(buf)){
            perror("fwrite");
            exit(EXIT_FAILURE);
        }
        fclose(stream);

    }
    return 0;


}
