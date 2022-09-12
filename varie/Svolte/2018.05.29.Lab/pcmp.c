#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> 
#include <stdlib.h>
#include <errno.h>  
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>
#include <string.h>
#include <grp.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <sys/prctl.h>
#include <pthread.h>
#include <time.h>
#include <sys/eventfd.h>
#include <sys/inotify.h>
#include <sys/signalfd.h>

typedef struct process_status
{
    pid_t pid;
    int status;
} p_s;

/*A volte non funziona se i file differiscono di una sola lettera. Probabilmente dipende
 *dalla divisione dei byte nelle righe 52/53
 */

int main(int argc, char const *argv[])
{
    /*prendiamo il numero di processi che vogliamo usare per comparare*/
    int process = atoi(argv[1]);
    int fd[2];
    /*creiamo una pipe che ci servira' per comunicare i risultati*/
    int pipefd[2];
    pipe(pipefd);
    /*creiamo un array per gestire gli stati (se il processo e' terminato) e per mantenere i pid*/
    p_s children[process];
    /*creiamo i processi in parallelo*/
    for (int i = 0; i < process; i++)
    {
        /*se e' il figlio*/
        if ((children[i].pid = fork()) == 0)
        {
            /*indica che non ha finito*/
            children[i].status = 0;
            /*chiude la pipe lato lettura*/
            close(pipefd[0]);
            /*collega lo standard output al lato lettura della pipe*/
            dup2(pipefd[1], STDOUT_FILENO);
            /*chiude il fd ridondante*/
            close(pipefd[1]);
            struct stat info[2];
            /*apre i file*/
            fd[0] = open(argv[2], O_RDONLY);
            fd[1] = open(argv[3], O_RDONLY);
            /*legge gli attributi*/
            fstat(fd[0], &info[0]);
            fstat(fd[1], &info[1]);
            /*il buffer deve avere una dimensione pari a quello che spetta da leggere
             *a questo processo + 1 (per il terminatore di stringa)*/
            char buffer1[info[0].st_size / process + 1];
            char buffer2[info[1].st_size / process + 1];
            /*calcolo offset*/
            off_t offset[2];
            offset[0] = i * info[0].st_size / process;
            offset[1] = i * info[1].st_size / process;
            /*legge i file tutti in una volta*/
            pread(fd[0], buffer1, sizeof(buffer1), offset[0]);
            close(fd[0]);
            pread(fd[1], buffer2, sizeof(buffer2), offset[1]);
            close(fd[1]);
            /*aggiunge terminatore di stringa*/
            buffer1[info[0].st_size / process] = '\0';
            buffer2[info[0].st_size / process] = '\0';
            /*se sono diversi scrive -1 alla pipe, altrimenti 0*/
            if (strcmp(buffer1, buffer2) != 0)
            {
                char return_val[] = "-1";
                write(STDOUT_FILENO, return_val, sizeof(return_val));
            }
            else
            {
                char return_val[] = "0";
                write(STDOUT_FILENO, return_val, sizeof(return_val));
            }
            exit(0);            
        }
    }
    /*il padre chiude lato scrittura pipe e sposta lo standard in sul lato lettura della pipe*/
    close(pipefd[1]);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    /*per ogni processo creato*/
    for (int i = 0; i < process; i++)
    {
        int status;
        /*aspetta un figlio*/
        pid_t child = wait(&status);
        /*ne imposta lo status a 1 (terminato)*/
        for (int j = 0; j < process; j++)
        {
            if (children[j].pid == child)
            {
                children[j].status = 1;
            }         
        }
        /*legge la pipe*/        
        char return_val[5];
        read(STDIN_FILENO, return_val, sizeof(return_val));
        /*se il figlio ha detto che ci sono differenze uccide tutti i processi ancora in corso
         *e stampa*/
        if (strcmp(return_val, "-1") == 0)
        {
            for (int j = 0; j < process; j++)
            {
                if (children[j].status == 0)
                {
                    kill(children[j].pid, SIGINT);
                }             
            }
            printf("%s %s diff\n", argv[2], argv[3]);
            return EXIT_FAILURE;
        } 
    }
    /*file uguali*/    
    return EXIT_SUCCESS;
}
