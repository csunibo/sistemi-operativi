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

/*serve come numero abbastanza grande da poter contenere un comando
 *piu' tutte le opzioni piu NULL
 */
#define MAXOPTIONS 10

int main(int argc, char *argv[])
{
    /*numero di figli da aspettare*/
    int num_of_commands = 0;

    /*contiene il comando piu' le opzioni piu' NULL da passare a exec*/
    char *command[MAXOPTIONS];

    /*cicliamo per tutti gli argomenti passati
     *(tranne il primo che e' il nome dell'eseguibile)*/
    for (int i = 1; i < argc; i++)
    { 
        int j = 0;

        /*prendiamo l'intero comando (// e' il separatore dei comandi)*/
        while (i < argc && strcmp(argv[i], "//") != 0)
        {
            command[j] = argv[i];
            j++;
            i++;
        }
        /*deve terminare con NULL il vettore che passiamo a exec*/
        command[j] = NULL;

        /*nuovo comando da aspettare*/
        num_of_commands++;

        /*nuovo figlio*/
        pid_t pid = fork();

        if (pid == 0)
        {
            /*esegue il comando*/
            execvp(command[0], command);
            /*in teoria qui' non dovrebbe mai arrivarci*/
            exit(1);
        }
    }
    
    /*aspetta tutti i figli*/
    for (int i = 0; i < num_of_commands; i++)
    {
        int status;
        wait(&status);
    }
    
    return 0;
}
