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
#include <sys/timerfd.h>

int main(int argc, char const *argv[])
{   
    /*convertiamo stringa in int*/
    int milliseconds = atoi(argv[1]);
    /*impostiamo il timer*/
    struct itimerspec new_value;
    new_value.it_value.tv_sec = (milliseconds * 1000000) / 1000000000;
    new_value.it_value.tv_nsec = (milliseconds * 1000000) % 1000000000;
    new_value.it_interval.tv_sec = 0;
    new_value.it_interval.tv_nsec = 0;
    /*creiamo un fd*/
    int timerfd = timerfd_create(CLOCK_REALTIME, 0);
    /*settiamo il timer*/
    if (timerfd_settime(timerfd, 0, &new_value, NULL) == -1)
    {
        perror("error on settime: ");
        exit(EXIT_FAILURE);
    }
    uint64_t expirations;
    printf("Starting at %d...\n", (int)time(NULL));
    /*quando scade si sblocca*/
    expirations = read(timerfd, &expirations, sizeof(uint64_t));
    if (expirations > 0)
    {
        printf("Expired at %d.\n", (int)time(NULL));
    }
    
    return 0;
}
