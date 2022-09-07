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
#include <sys/signalfd.h>

#define MAXFILE 10000

/*funziona :)*/

int main(int argc, char const *argv[])
{   
    struct stat info;
    int fd = open(argv[1], O_RDONLY);
    fstat(fd, &info);
    char buffer[MAXFILE];
    read(fd, buffer, MAXFILE);
    int oldtime, newtime, first = 1;
    int i = 0;
    while (i < strlen(buffer)) 
    {
        char row[MAXFILE];
        int offset = 7;
        char hours[3];
        char minutes[3];
        char seconds[3];
        hours[0] = buffer[i + offset];
        hours[1] = buffer[i + offset + 1];
        hours[2] = '\0';
        minutes[0] = buffer[i + offset + 3];
        minutes[1] = buffer[i + offset + 4];
        minutes[2] = '\0';
        seconds[0] = buffer[i + offset + 6];
        seconds[1] = buffer[i + offset + 7];
        seconds[2] = '\0';
        newtime = atoi(hours) * 60 * 60 + atoi(minutes) * 60 + atoi(seconds);
        int j = i;
        while (buffer[j] != '\n' && buffer[j] != '\0')
        {
            j++;
        }

        for (int k = 0; i <= j; k++)
        {
            row[k] = buffer[i];
            i++;
            if (k == j)
            {
                row[k + 1] = '\0';
            }           
        }
        
        if (first)
        {
            printf("%s", row);
            oldtime = newtime;
            first = 0;
        }
        else
        {
            sleep(newtime - oldtime);
            printf("%s", row);
            oldtime = newtime;
        }
        /*pulisce il buffer*/
        memset(row,0,strlen(row));
    }
    printf("\n");
    

    return 0;
}
