/* Lo scopo di questo esercizioè di scrivere msleep. msleep si comporta come sleep(1) ma invece di attendere il */
/* numero di secondi indicati come parametro deve aspettare il numero di millisecondi indicati come parametro.d */
/* es: */
/* msleep 2340 */
/* completa la sua esecuzione in 2340 millisecondi alias 2.34 secondi. */
/* La msleep deve essere implementata usando i timerfd (v. timerfd_create(2)). */
/* Estendere la soluione dell'esercizio 1: mmsleep (multimsleep) ammette molteplici parametri. */
/* msleep 1234 3456 2345 */
/* allo scadere di 1234 millisecondi stampa 1234, allo scadere di 2345 millisecondi stampa 2345 e infine a 3456 */
/* millisecondi dalla attivazione stampa 3456 ed esce. */
/* Questo programma deve essere implementato creando un timerfd per ogni parametro e usando poll(2) per */
/* aspettare il prossimo evento. */
#include <bits/types/struct_itimerspec.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/timerfd.h>
#include <stdint.h>        /* Definition of uint64_t */
#include <sys/wait.h>
#include <sys/poll.h>
#include <pthread.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
void HANDLE_ERROR(const char *msg){
    do { perror(msg); exit(EXIT_FAILURE); } while (0);
}

typedef struct structarg {
    int fd;
    struct itimerspec new_value;
    char *to_print;
    int pipe_end;
} structarg_t;
//gli passeremo structarg
void *thread(void *arg){
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd == -1)
        HANDLE_ERROR("timerfd_create");
    structarg_t argu = *( (structarg_t *) arg);
    /* int fd = argu.fd; */
    if (write(argu.pipe_end, &fd, sizeof(int))<0){
        HANDLE_ERROR("\nwrite()");
    }

    struct itimerspec new_value = argu.new_value;
    printf("\n%s started", argu.to_print);
    if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL) == -1)
        HANDLE_ERROR("timerfd_settime");
    printf("\n%s expired", argu.to_print);
    exit(EXIT_SUCCESS);
}
enum _PIPE {READ, WRITE};
int main(int argc, char *argv[]){

    //pipe creata per "comunicare" i fd creati dinamicamente
    int fds[argc-1][2];
    for (int p=0; p<argc-1; p++){
        if (pipe(fds[p])<0){
            HANDLE_ERROR("\npipe()");
        }
    }


    if (argc < 2){
        fprintf(stderr, "\nBAD USAGE");
        exit(EXIT_FAILURE);
    }

    struct pollfd poll_list[argc-1]; bzero(poll_list, sizeof(poll_list));

    //ad ogni thread passeremo due parametri
    /* pthread_t threads[argc-1]; bzero(threads, sizeof(threads)); */
    structarg_t args[argc -1]; bzero(args, sizeof(args));

    for (int i = 1; i< argc; i++){
        struct timespec now; bzero(&now, sizeof(struct timespec));
        if (clock_gettime(CLOCK_REALTIME, &now) == -1) HANDLE_ERROR("clock_gettime");
        /* Create a CLOCK_REALTIME absolute timer with initial expiration */
        struct itimerspec new_value; bzero(&new_value, sizeof(struct itimerspec));
        long converted = atoi(argv[i])*(10e5); //da millisecondi a nanosec
        printf("converted: %ld\n", converted);
        /* https://stackoverflow.com/questions/65341435/reversing-last-n-digits-of-an-integer-in-c */
        int secs = converted / 10e8;
        long nsecs = converted - secs*10e8;
        fprintf(stdout, "\nsecs : %d, nsecs = %lu", secs, (unsigned long) nsecs);
        /* long nsecs = 0; */
        /* int secs = 2; */
        new_value.it_value.tv_sec = now.tv_sec + secs;
        new_value.it_value.tv_nsec = nsecs;
        new_value.it_interval.tv_sec = 0;
        new_value.it_interval.tv_nsec = 0;
        /* int fd = timerfd_create(CLOCK_REALTIME, 0); */
        /* if (fd == -1) */
        /*     HANDLE_ERROR("timerfd_create"); */
        structarg_t arg; bzero(&arg, sizeof(arg));
        /* arg.fd = fd; */
        arg.new_value = new_value;
        arg.pipe_end = fds[i-1][WRITE];
        arg.to_print = argv[i];
        args[i-1] = arg;
        /* poll_list[i-1].fd = fd; */
        poll_list[i-1].events = POLLIN;
    }

    for (int j = 1; j < argc; j++){
        pid_t pid;
        pid = fork();
        if (pid == -1) HANDLE_ERROR("\nfork()");
        else if (pid == 0){
            thread((void *)&args[j-1]);
        }
        else{
            int fd;
            close(fds[j-1][WRITE]);
            if(read(fds[j-1][READ], &fd, sizeof(int))<0){
                HANDLE_ERROR("\nread()");
            }
            printf("\nRICEVUTO");
            poll_list[j-1].fd = fd;
        }
        /* pthread_create(&threads[j-1], NULL, thread, (void *)&args[j-1]); */
    }

    //ci aspettiamo di ricevere argc-1 eveneti
    for (int k=0; k < argc -1; k++){
        uint64_t exp = 0;
        ssize_t s = read(args[k].fd, &exp, sizeof(uint64_t));
        if (s != sizeof(uint64_t)){
            HANDLE_ERROR("\nread()");
        }

        /* int nfds = poll(poll_list, argc-1, -1); */
        /* if (nfds == -1){ */
        /*     HANDLE_ERROR("\npoll()"); */
        /* } */
    }
    for (int n=0; n < argc-1; n++){
        /* pthread_join(threads[n], NULL); */
        wait(NULL);
    }
    /* uint64_t exp = 0; */
    /* ssize_t s = read(fd, &exp, sizeof(uint64_t)); */
    /* if (s != sizeof(uint64_t)){ */
    /*     HANDLE_ERROR("\nread()"); */
    /* } */
    exit(EXIT_SUCCESS);
}
