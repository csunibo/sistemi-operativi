#include <signal.h>
#include <sys/signalfd.h>
#include <stdio.h>
#include <unistd.h>
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
#include <stdlib.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define TRUE 1
#define FALSE 0
#include <strings.h>
#define MAX_STR_LEN 4096
#include <string.h>
void send_char(char c, pid_t pid, int fd){
    const size_t size = sizeof(char);
    const int BITS = size*8;
    char bin[BITS+1]; bzero(bin, BITS + 1); bin[BITS] = '\0';
    int k =0;
    for (int i=0; i < size; i++){ //for each byte
        char byte = c;
                                     //il linguaggio C in sè non è nè BIG ne little ENDIAN. Tutto dipende da cosa noi prendiamo (il bit meno significativo) ;)
        for (int j = 0; j < 8; j++){//for each bit. Starting from 0 in order to get the least significant bit. Si parte a leggere l'iesimo byte DA DESTRA. IN MODO DA PRINTARE LA CORRETTA RAPPRESENTAZIONE => NON 1101 ALLA BONONI MA 1011. SE BIG ENDIAN => j = 7. Big ENDIAN per assurdo è già la versione reversata del carattere. Quindi una volta invertita ritorna il carattere passato come input
            int bit = (byte >> j) & 1; //check last bit with & 000..01
            if (bit == 1){
                printf("1");
                kill(pid, SIGUSR1);
            }
            else if (bit == 0){
                printf("0");
                kill(pid, SIGUSR2);
            }
            //aspettiamo riscontro
            struct signalfd_siginfo fdsi;// bzero(&fdsi, sizeof(struct signalfd_siginfo));
            if (read(fd, &fdsi, sizeof(struct signalfd_siginfo)) != sizeof(struct signalfd_siginfo)) HANDLE_ERROR("read");
            /* printf("%d", bit); //voglio printare il numero, non il carattere ASCII //LITTLE ENDIAN */
            sprintf(&bin[k], "%d", bit); //"half of half"
            k++;
        }
    }
    printf("%c", (char) strtol(bin, (char **)NULL, 2));
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
    for (int i=0; i < strlen(argv[2]); i++){
        send_char(argv[2][i], pid, fd);
    }
    for (int i = 0; i < 8; i++) kill(pid, SIGUSR1);
}
