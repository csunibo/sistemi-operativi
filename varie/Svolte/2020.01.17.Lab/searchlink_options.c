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

/*abspath del file da cercare*/
char *abspath;

/* options[0] = -l
 * options[1] = -c
 * options[2] = -s
*/
void listdir(char *dir_path, ino_t inode, char *relpath, int options[], char *realName)
{
    DIR *dir;
    struct dirent *entry;
    struct stat info;

    if (chdir(dir_path) == -1)
    {
        perror("error chdir: ");
    }

    if ((dir = opendir(".")) == NULL)
    {
        perror("error opendir: ");
    }

    while ((entry = readdir(dir)) != NULL)
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
                /*caso effettua copia, apre il file, lo legge e lo scrive in un nuovo file*/
                if (options[1] == 1 && (strcmp(entry->d_name, realName) != 0))
                {
                    /*buffer di grandezza ottimale*/
                    char buffer[info.st_blksize];
                    int fd = open(entry->d_name, O_RDONLY);
                    /*creazione nuovo file*/
                    int fd_new = open("tmp", O_CREAT | O_WRONLY, 0777);
                    while (read(fd, &buffer, info.st_blksize) > 0)
                    {
                        write(fd_new, &buffer, info.st_blksize);
                    }
                    close(fd);
                    close(fd_new);
                    char *name = entry->d_name;
                    unlink(entry->d_name);
                    rename("tmp", name);    
                }
                /*se l'opzione -l era attiva e non e' il file originale*/
                else if (options[0] == 1 && (strcmp(entry->d_name, realName) != 0))
                {
                    /*unlinka il sym link e trasformalo in un hardlink*/
                    char *name = entry->d_name;
                    unlink(entry->d_name);
                    link(abspath, entry->d_name);
                }
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
                /*caso copia analogo a sopra*/
                if (options[1] == 1 && (strcmp(entry->d_name, realName) != 0))
                {
                    char buffer[info.st_blksize];
                    int fd = open(entry->d_name, O_RDONLY);
                    int fd_new = open("tmp", O_CREAT | O_WRONLY, 0777);
                    while (read(fd, &buffer, info.st_blksize) > 0)
                    {
                        write(fd_new, &buffer, info.st_blksize);
                    }
                    close(fd);
                    close(fd_new);
                    char *name = entry->d_name;
                    unlink(entry->d_name);
                    rename("tmp", name);    
                }
                /*se l'opzione -s era attiva*/
                if (options[2] == 1 && (strcmp(entry->d_name, realName) != 0))
                {
                    /*trasforma l'hard link in un symlink*/
                    char *name = entry->d_name;
                    unlink(entry->d_name);
                    symlink(abspath, name);
                }
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
                listdir(entry->d_name, inode, dest, options, realName);
                /*serve a farlo ritornare altrimenti scandisce due volte e da errori
                 *perche' nel mentre cambia la current dir*/
                exit(0);
                break;
            default:
                ;
                int status;
                waitpid(pid, &status, 0);
                break;
            }
        }
    }
    closedir(dir); 
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
                abspath = realpath(entry->d_name, NULL);
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
    ino_t inode = find_inode(argv[argc - 2], argv[argc - 1]);
    int options[3] = {0, 0, 0};

    /*non esiste nessun file chiamato cosi' in questo albero*/
    if (inode == -1)
    {
        exit(EXIT_FAILURE);
    }

    /*se sono state passate opzioni attiva quelle passate*/
    if (argc > 3)
    {
        for (int i = 0; i < argc; i++)
        {
            if (strcmp(argv[i], "-l") == 0)
            {
                options[0] = 1;
            }
            
            else if (strcmp(argv[i], "-c") == 0)
            {
                options[1] = 1;
            }
            
            else if (strcmp(argv[i], "-s") == 0)
            {
                options[2] = 1;
            }
        }
    }
    
    /*cerca tutti i link*/
    listdir("./", inode, ".", options, argv[argc - 2]);

    return 0;
}
