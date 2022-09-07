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

/*definizioni per usare la struttura inotyfy*/
#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))


int main(int argc, char *argv[])
{
    /*crea un fd per inotify e lo mette a guardare la directory*/
    int fd = inotify_init();
    int wd = inotify_add_watch(fd, argv[1], IN_CREATE);

    if (wd == -1)
    {
        perror("notify add error: ");
    }
    else
    {
        printf("Start watching %s\n", argv[1]);
    }
    
    while (1)
    {
        int length, i = 0;
        char buffer[BUF_LEN];
        
        /*legge il contenuto di notify*/
        length = read(fd, buffer, BUF_LEN);

        while (i < length)
        {

            struct inotify_event *event = (struct inotify_event*) &buffer[i];

            if (event->len)
            {
                /*vede se e' stato creato un nuovo file*/
                if (event->mask & IN_CREATE)
                {
                    /*lavoro con le stringhe per creare il path da eseguire*/
                    printf("File %s created\n", event->name);
                    char path[100];
                    strcpy(path, argv[1]);
                    char *to_execute;
                    pid_t pid;
                    to_execute = strcat(path, event->name);
                    /*fork*/
                    if ((pid = fork()) == 0)
                    {   
                        /*esegue il programma appena creato*/
                        execl(to_execute, to_execute, NULL);
                        exit(0);
                    }
                    else
                    {
                        /*il padre cancella il file quando il figlio ritorna*/
                        int status;
                        waitpid(pid, &status, 0);
                        unlink(to_execute);
                    }     
                }
            }

            i += EVENT_SIZE + event->len;
        }
    }
        
    inotify_rm_watch(fd, wd);
    close(fd);


    return 0;
}
