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

/*per info usare ./ager --help*/

void invecchia(char path_name[], int age, int mtim, int atim)
{
    struct stat info;
    int fd;
    if (fd = open(path_name, O_WRONLY) == -1)
    {
        perror("open error ");
    }
    if (fstat(fd, &info) == -1)
    {
        perror("stat error: ");
    }
    struct timeval times[2];
    if (atim)
    {
        times[0].tv_sec = info.st_atim.tv_sec + age;
        times[0].tv_usec = 0;
    }
    else
    {
        times[0].tv_sec = info.st_atim.tv_sec;
        times[0].tv_usec = 0;
    }
    if (mtim)
    {
        times[1].tv_sec = info.st_mtim.tv_sec + age;
        times[1].tv_usec = 0;
    }
    else
    {
        times[1].tv_sec = info.st_mtim.tv_sec;
        times[1].tv_usec = 0;
    }
    
    if (utimes(path_name, times) == -1)
    {
        perror("utime error: ");
    }
    close(fd);
}

void change_time_dir(int atim, int mtim, int age)
{
    DIR *fdDir;
            fdDir = opendir("./");
            struct dirent *entry;
            while ((entry = readdir(fdDir)) != NULL)
            {
                struct stat info;
                stat(entry->d_name, &info);
                if (!S_ISDIR(info.st_mode))
                {
                    invecchia(entry->d_name, age, mtim, atim);
                }    
            }    
}

int main(int argc, char *argv[])
{
    int atim = 1;
    int mtim = 1;
    int age = 864000;
    int options = 0;
    int manual_time = 0;
    if (argc == 1)
    {
        change_time_dir(atim, mtim, age);
    } 

    else
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            printf("./ager [options] [tempo modifica] [files]\n");
            printf("-a modifica solo ultimo accesso\n");
            printf("-m modifica solo ultima modifica\n");
            printf("-t si intende impostare un tempo manualment (si specifica in ore)\n");
            printf("Se non e' specificato nessun tempo di modifica cambia in +10 giorni\n");
            printf("Se non sono specificati argomenti cambia tutti i file in +10 giorni\n");
            return(0);
        }
        if (strstr(argv[1], "-") != NULL)
        {
            if (strstr(argv[1], "a") != NULL)
            {
                mtim = 0;
            }
            else if (strstr(argv[1], "m") != NULL)
            {
                atim = 0;
            }
            if (strstr(argv[1], "t") != NULL)
            {
                manual_time = 1;
            }
            options = 1;   
        }
        int i = options + 1;
        if (manual_time == 1)
        {
            age = atoi(argv[2]) * 60 * 60;
            i++;
        }

        for (i; i < argc; i++)
        {
            
            invecchia(argv[i], age, mtim, atim);
        }
        
    }
    
    
    
    return 0;
}
