#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> 
#include <stdlib.h>
#include <errno.h>  
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>                              //qui e' definito PATH_MAX
#include <string.h>
#include <grp.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{    
    char buff_rel[PATH_MAX];                     //buffer per il path relativo del file
    char buff_abs[PATH_MAX];                     //buffer per il path assoluto
    char new_link_name[PATH_MAX];                //buffer per il nome del nuovo link
    char *ptr;
    
    /*leggiamo il contenuto del link*/
    if (readlink(argv[1], buff_rel, PATH_MAX) == -1)
    {
        perror("Error on readlink: ");
    }      

    //printf("Rel path is: %s\n", buff_rel);

    /*otteniamone il path assoluto*/
    if ((ptr = realpath(buff_rel, buff_abs)) == NULL)
    {
        perror("Error on realpath: ");
    }         

    //printf("Abs path is: %s\n", ptr);

    /*copiamo il nome del vecchio link in un buffer abbastanza grande*/
    strncpy(new_link_name, argv[1], PATH_MAX);  

    /*concateniamo "_abs" al vecchio nome*/
    char *newLinkPtr = strcat(new_link_name, "_abs");

    /*eliminiamo eventuali link pre-esistenti con questo nome*/
    unlink(newLinkPtr);  

    /*creiamo un nuovo link simbolico*/
    if (symlink(ptr, newLinkPtr) == -1)
    {
        perror("Error on symlink: ");
    }                 

    return 0;
}
