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



typedef struct info
{
    ino_t inode;
    int link;
} iinfo;

int current = 0;
iinfo inode[MAXFILE];

int searchRec(char *path)
{
    int total = 0;
    if (chdir(path) == -1)
    {
        perror("chdir: ");
    }
    DIR *dirStream = opendir(".");
    struct dirent *entry;
    while ((entry = readdir(dirStream)) != NULL)
    { 
        struct stat info; 
        if (lstat(entry->d_name, &info) == -1)
        {
            perror("stat error: ");
        }
        if (S_ISDIR(info.st_mode) && (strcmp(entry->d_name, ".") != 0) && (strcmp(entry->d_name, "..") != 0))
        {   
            total = total + searchRec(entry->d_name) + 1;
            chdir("..");
        }
        else if (S_ISREG(info.st_mode))
        {
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
                total++;
                inode[current].inode = info.st_ino;
                inode[current].link = 1;
                current++;
            }    
        }  
    }
    closedir(dirStream);
    return total;    
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("%d\n", searchRec("."));
    }
    else
    {
        printf("%d\n", searchRec(argv[1]));
    }
    return 0;
}
