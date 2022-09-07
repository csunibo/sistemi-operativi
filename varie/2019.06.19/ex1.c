/* Normalmente se un processo è orfano l'exit status viene ricevuto da init o systemd (processo 1). */
/* Usando l'opzione PR_SET_CHILD_SUBREAPER della system call prctl(2) è possibile cambiare questo */
/* comportamento. */
/* Scrivere un programma che crei un processo figlio e uno nipote (tramite fork). Fare in modo che */
/* termini prima il figlo e poi il nipote. */
/* Il programma deve mostrare che con prctl/PR_SET_CHILD_SUBREAPER la terminazione del nipote */
/* viene rilevata dal nonno. */

/* Tramite l'opzione PR_SET_NAME sempre di prctl fare in modo che con il comando "ps -Leo pid,comm" */
/* i nomi dei tre rpocessi creati dall'esercizio 1 compaiano con nome "nonno","figlio" e "nipote". */

/* https://stackoverflow.com/questions/56856275/is-there-some-short-example-of-how-to-use-prctl-when-it-comes-to-setting-subre */
/* https://stackoverflow.com/questions/32004492/does-the-child-subreaper-bit-persist-across-fork */
/* https://github.com/AngeloGalav/UniOSScripts/blob/main/2019.06.19/es2/famiglia.c */
/* https://linuxhint.com/use-prctl-c/ */ //per SET NAME
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <string.h>
#include <grp.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <sys/prctl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    /*il PR_SET_CHILD_SUBREAPER non viene ereditato dalla fork*/
    prctl(PR_SET_CHILD_SUBREAPER, 1, 0, 0, 0, 0);

    prctl(PR_SET_NAME, "Nonno", 0, 0, 0, 0);
    /*creazione primo figlio*/
    if (fork() == 0)
    {
        /*creazione nipote*/
        if (fork() != 0) //creato un altro pid (il figlio tiene ancora 0)
        {
            prctl(PR_SET_NAME, "Figlio", 0, 0, 0, 0);
            sleep(1);
            printf("First child ends, pid: %d, parent: %d\n", getpid(), getppid());
            exit(0);
        }
        else
        {
            prctl(PR_SET_NAME, "Nipote", 0, 0, 0, 0);
            printf("Granchild starts, pid: %d, parent: %d\n", getpid(), getppid());
            sleep(2);
            printf("Granchild ends, pid: %d, parent: %d\n", getpid(), getppid());
            exit(0);
        }
    }

    /*il padre aspetta entrambi*/
    else
    {
        for (int i = 0; i < 2; i++)
        {
            int status;
            pid_t pid = wait(&status);

            printf("wait for pid: %d\n", pid);
        }

    }

    return 0;
}

