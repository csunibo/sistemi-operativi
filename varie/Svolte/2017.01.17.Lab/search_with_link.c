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
#include <sys/time.h>
#include <sys/eventfd.h>
#include <sys/inotify.h>
#include <sys/signalfd.h>

#define MAXFILE 100000


/*struttura per contare il numero di link per ogni inode*/
typedef struct info
{
    ino_t inode;
    int link;
} iinfo;

/*posizione corrente nell array e array di iinfo*/
int current = 0;
iinfo inode[MAXFILE];

void searchRec(char *path)
{
    /*cambiamo la dir corrente*/
    if (chdir(path) == -1)
    {
        perror("chdir: ");
    }
    /*la apriamo*/
    DIR *dirStream = opendir(".");
    struct dirent *entry;
    /*finche vi sono entry*/
    while ((entry = readdir(dirStream)) != NULL)
    { 
        /*prendiamo le stat (non segue i symlink)*/
        struct stat info; 
        if (lstat(entry->d_name, &info) == -1)
        {
            perror("stat error: ");
        }
        /*se e' una dir la scansiona (ricorsivamente) e torna poi a questa come corrente*/
        if (S_ISDIR(info.st_mode) && (strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0))
        {   
            searchRec(entry->d_name);
            chdir("..");
        }
        /*se e' un file normale*/
        else if (S_ISREG(info.st_mode))
        {
            /*se esiste gia questo inode incrementa di 1 i link counter, altrimenti aggiunge all array*/
            int found = 0;
            for (int i = 0; i <= current; i++)
            {
                if (info.st_ino == inode[i].inode)
                {
                    inode[i].link++;  
                    found = 1; break;
                }                
            }
            if (found == 0)
            {
                inode[current].inode = info.st_ino;
                inode[current].link = 1;
                current++;
            }    
        }  
    }
    closedir(dirStream);   
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        searchRec(".");
    }
    else
    {
        searchRec(argv[1]);
    }
    /*calcola l'ampiezza dell'array in base al massimo numero di link*/
    int max = 0;
    for (int i = 0; i < current; i++)
    {
        if (inode[i].link > max)
        {
            max = inode[i].link;
        }
        
    }
    /*inizializza l'array a 0*/
    int links[max];
    for (int i = 0; i < max; i++)
    {
        links[i] = 0;
    }
    /*aumenta di 1 il numero di file con inode[i].link - 1 link*/
    for (int i = 0; i < current; i++)
    {
        links[inode[i].link - 1]++;
    }
    /*stampa*/
    for (int i = 0; i < max; i++)
    {
        if (links[i])
        {
            printf("Numero files con %d link: %d\n", i+1, links[i]);
        }
        
        
    }
    
    
    
    return 0;
}
