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

int main(int argc, char const *argv[])
{
    //printf("Directory da esaminare: %s\n", argv[1]);
    DIR *directory;                                           //directory stream
    struct dirent *entry;                                     //directory entry
    char buff_abs[PATH_MAX];
    char *abs_path;

    /*cambia la directory corrente in quella passata come parametro*/
    if (chdir(argv[1]) == -1)
    {
        perror("chdir error: ");
    }
    
    /*apre la directory da esaminare*/
    if ((directory = opendir(argv[1])) == NULL)
    {
        perror("opendir error: ");
    }

    /*legge la directory fino alla fine*/
    while ((entry = readdir(directory)) != NULL)
    {
        /*ottiene il path assoluto*/
        if ((abs_path = realpath(entry->d_name, buff_abs)) == NULL)
        {
            perror("Error on realpath: ");
        }

        /*lo stampa*/
        printf("Entry %s abs path is %s\n", entry->d_name, abs_path);   

    }
    
    /*chiude la directory*/
    closedir(directory);

    return 0;
}
