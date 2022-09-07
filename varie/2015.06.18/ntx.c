/* Usando signalfd (e non signal o sigaction) scrivere due programmi tx e rx. */
/* Rx deve essere attivato per primo e per prima cosa stampa il suo pid e aspetta. */
/* Tx ha due parametri: il pid del ricevente rx e una stringa. Rx deve stampare la stringa passata come secondo */
/* parametro a Tx e uscire. La stringa deve essere spedita usando solo segnali. Ogni bit di ogni carattere deve essere */
/* inviato usando SIGUSR1 se vale 1 e SIGUSR2 se vale 0. Al ricevimento di ogni bit rx spedisce un segnale di */
/* acknowledgment a tx che provvede a spedire il successivo. */
/* SI costruiscano due programmi txtime e rxnull per stimare l'efficienza di tx e rx. Rxnull e' uguale all'rx precedente */
/* ma non stampa nulla. */
/* Txtime ha come parametri il pid del ricevente e un numero intero n. txtime deve spedire n caratteri 'K' al txnull e alla */
/* fine deve stampare il tempo medio impiegato per spedire n carattere. */
/* https://stackoverflow.com/questions/41384262/convert-string-to-binary-in-c */
/* https://www.eso.org/~ndelmott/ascii.html */
/* https://www.geeksforgeeks.org/how-to-measure-time-taken-by-a-program-in-c/ */
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#include <strings.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/signalfd.h>
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
char* stringToBinary(char* s) {
    if(s == NULL) return 0; /* no input string */
    size_t len = strlen(s);
    char *binary = malloc(len*8 + 1); // each char is one byte (8 bits) and + 1 at the end for null terminator
    binary[len*8] = '\0';
    int k=0;
    for(size_t i = 0; i < len; ++i) {
        char ch = s[i];
        /* for(int j = 7; j >= 0; j--){ */
        for (int j=0; j< 8; j++){
            int bit = (ch >> j) & 1;
            sprintf(&binary[k], "%d", bit);
            /* if((1 << j) & ch) { */
            /*     sprintf(&binary[k], "%d", 1); */
            /* } else { */
            /*     sprintf(&binary[k], "%d", 0); */
            /* } */
            k++;
        }
    }
        return binary;
    }

    int main(int argc, char **argv){
        if (argc != 3){
            fprintf(stderr, "BAD USAGE\n");
            exit(EXIT_FAILURE);
        }
        pid_t pid = (pid_t) atoi(argv[1]);
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) HANDLE_ERROR("sigprocmask");
        int fd = signalfd(-1, &mask, 0);
        if (fd == -1) HANDLE_ERROR("signalfd");
        char K[atoi(argv[2])+1]; INIT_ARRAY(K); K[atoi(argv[2])] = '\0';
        for (int i=0; i< atoi(argv[2]); i++){
            sprintf(&K[i], "%c", 'K');
        }
        char *bin = stringToBinary(K);
        clock_t t = clock();
        for (int i=0; i < strlen(bin) + 1; i ++){ //se inviassimo anche il NULL terminator? NON HA FUNZIONATO!
            if (bin[i] == '0') kill(pid, SIGUSR2);
            else if (bin[i] == '1') kill(pid, SIGUSR1);
            else {
                for (int j=0; j< 8; j++) {
                    kill(pid, SIGUSR2);
                    struct signalfd_siginfo si;
                    if (read(fd, &si, sizeof(si)) != sizeof(si)) HANDLE_ERROR("read");
                }
                t = clock() - t;
                double time_taken = ( ((double)t)/CLOCKS_PER_SEC )/atoi(argv[2]); // in seconds

                printf("tempo media impiegato float: %f\n", time_taken);
                free(bin);
                exit(EXIT_SUCCESS);
            }
            //ACK
            struct signalfd_siginfo fdsi;
            if (read(fd, &fdsi, sizeof(fdsi)) != sizeof(fdsi)) HANDLE_ERROR("read");
        }
        t = clock() - t;
        double time_taken = ( ((double)t)/CLOCKS_PER_SEC )/atoi(argv[2]); // in seconds

        printf("tempo media impiegato float: %f\n", time_taken);
        free(bin);
        /* for (int i = 0; i < 8; i++) { */
        /*     union sigval value; */
        /*     value.sival_ptr = (void*)NULL; */
        /*     if (sigqueue(pid, SIGUSR2, value) != 0) HANDLE_ERROR("sigqueue"); */
        /*     /1* kill(pid, SIGUSR2); *1/ */
        /*     /1* sleep((double)0.5); //aggiungendo uno sleep funziona... SIGQUEUE is the way?? *1/ */
        /* } */
        exit(EXIT_SUCCESS);
    }
