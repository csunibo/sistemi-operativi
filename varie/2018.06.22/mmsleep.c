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
#include <poll.h>

/*funzione ausiliaria per ordinamento*/
int comp (const void * elem1, const void * elem2)
{
    long f = *((long*)elem1);
    long s = *((long*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

int main(int argc, char const *argv[])
{
    long milliseconds[argc-1];
    struct itimerspec new_value[argc-1];
    int timerfd[argc-1];
    struct pollfd ufds[argc-1];
    /*riempiamo prima il vettore dei timer*/
    for (int i = 0; i < argc - 1; i++)
    {
        milliseconds[i] = atoi(argv[i + 1]);
    }
    /*lo ordiniamo*/
    qsort(milliseconds, sizeof(milliseconds)/sizeof(*milliseconds), sizeof(*milliseconds), comp);

    /*riempiamo le strutture e creiamo un fd*/
    for (int i = 0; i < argc - 1; i++)
    {
        new_value[i].it_value.tv_sec = (milliseconds[i] * 1000000) / 1000000000;
        new_value[i].it_value.tv_nsec = (milliseconds[i] * 1000000) % 1000000000;
        new_value[i].it_interval.tv_sec = 0;
        new_value[i].it_interval.tv_nsec = 0;
        timerfd[i] = timerfd_create(CLOCK_REALTIME, 0);
        if (timerfd_settime(timerfd[i], 0, &new_value[i], NULL) == -1)
        {
            perror("error on settime: ");
            exit(EXIT_FAILURE);
        }
        ufds[i].fd = timerfd[i];
        /*impostando questo significa che si sblocca quando c'e' qualcosa da leggere*/
        ufds[i].events = POLLIN;
    }
    printf("Starting at %d...\n", (int)time(NULL));
    /*facciamo poll mandando avanti man mano il vettore (che e' "ordinato")*/
    for (int i = 0; i < argc - 1; i++)
    {
        int fd = poll(ufds + i, argc - 1 - i, -1);
        printf("Expired at %d.\n", (int)time(NULL));

    }


    return 0;
}
