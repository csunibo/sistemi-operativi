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

#define DIRMAX 1000

/*cerca tutti i link e li stampa. relpath e' una stringa che indica
* i nodi dell'albero percorsi. Le prime parti sono molto simili
* a quelle della funzione find_inode che sono commentate (vedere sotto)
*/
void listdir(char *dir_path, ino_t inode, char *relpath)
{
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    int stop = 0;

    if (chdir(dir_path) == -1)
    {
        perror("error chdir: ");
    }

    if ((dir = opendir(".")) == NULL)
    {
        perror("error opendir: ");
    }

    while ((entry = readdir(dir)) != NULL && !stop)
    {
       
        if (stat(entry->d_name, &info) == -1)
        {
            perror("error stat: ");
            continue;
        }
        
        /*ha trovato un file con stesso inode*/
        if (info.st_ino == inode)
        {
            struct stat info2;
            /*lstat non segue i sym link*/
            lstat(entry->d_name, &info2);
            /*macro che controlla se e' un symlink*/
            if (S_ISLNK(info2.st_mode))
            {   
                /*operazioni per formattare l'output*/
                char src[PATH_MAX];
                char dest[PATH_MAX];
                strcpy(dest, relpath);
                strcpy(src, "/");
                strcpy(dest, relpath);
                strcat(dest, src);
                strcpy(src, entry->d_name);
                strcat(dest, src);
                printf("Sym link %s\n", dest);
            }
            else
            {
                char src[PATH_MAX];
                char dest[PATH_MAX];
                strcpy(dest, relpath);
                strcpy(src, "/");
                strcpy(dest, relpath);
                strcat(dest, src);
                strcpy(src, entry->d_name);
                strcat(dest, src);
                printf("Hard link %s\n", dest);
            }
        }
        
        /*se incontra una dir che non e' current o padre*/
        else if (S_ISDIR(info.st_mode) && (strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0))
        {
            /*la fa ispezionare ad un processo figlio*/
            pid_t pid = fork();
            switch (pid)
            {
            case 0:
                ;
                char src[PATH_MAX];
                char dest[PATH_MAX];
                strcpy(dest, relpath);
                strcpy(src, "/");
                strcat(dest, src);
                strcpy(src, entry->d_name);
                strcat(dest, src);
                listdir(entry->d_name, inode, dest);
                /*serve a farlo ritornare altrimenti scandisce due volte e da errori
                 *perche' nel mentre cambia la current dir*/
                stop = 1;
                break;
            default:
                ;
                int status;
                waitpid(pid, &status, 0);
                break;
            }
        }   
    }
}

/*cerca l'inode del file nel sottoalbero se esiste, altrimenti ritorna errore*/
ino_t find_inode(char* file_name, char *dir_path)
{
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    struct dirent *entries[DIRMAX];
    int i = 0;

    /*cambia current dir*/
    if (chdir(dir_path) == -1)
    {
        perror("error chdir: ");
    }

    /*apre la current dir*/
    if ((dir = opendir(".")) == NULL)
    {
        perror("error opendir: ");
    }

    while ((entry = readdir(dir)) != NULL)
    {
        /*prende le informazioni del file*/
        if (stat(entry->d_name, &info) == -1)
        {
            perror("error stat: ");
            continue;
        }

        /*se il file non e' una dir*/
        if (S_ISDIR(info.st_mode) == 0)
        {
            /*vede se fa match con il nome*/
            if (strcmp(file_name, entry->d_name) == 0)
            {
                return info.st_ino;
            }
        }

        /*altrimenti lo aggiunge alle dir incontrate in questa dir*/
        else if (S_ISDIR(info.st_mode) && (strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0))
        {
            entries[i] = entry;
            i++;
        }   
    }

    /*se il file non era in questa scandisce le dir ricorsivamente finche' lo trova o meno*/
    for (int j = 0; j < i; j++)
    {   
        ino_t result;
        if((result = find_inode(file_name, entries[j]->d_name)) != -1)
        {
            chdir("..");
            return result;
        }
        else
        {
            chdir("..");
        }
        
    }
    return -1;
    
}

int main(int argc, char *argv[])
{
    ino_t inode = find_inode(argv[1], argv[2]);

    /*non esiste nessun file chiamato cosi' in questo albero*/
    if (inode == -1)
    {
        exit(EXIT_FAILURE);
    }

    /*cerca tutti i link*/
    listdir("./", inode, ".");

    return 0;
}
