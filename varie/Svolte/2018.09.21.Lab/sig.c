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

char buffer[26];

/*funzione che aggiora il tempo nel buffer globale*/
void update_time()
{
    time_t timer;
    struct tm* tm_info;
    timer = time(NULL);
    tm_info = localtime(&timer);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
}

int main(int argc, char const *argv[])
{
    sigset_t mask;
    int sfd;
    int fd;

    /*creiamo un set dei segnali che vogliamo ricevere*/
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);

    /*impostiamo la maschera del processo corrente*/
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
    {
        perror("error sigprocmask "); exit(EXIT_FAILURE);
    }

    /*creiamo un fd per ricevere i segnali*/
    sfd = signalfd(-1, &mask, 0);

    if (sfd == -1)
    {
        perror("error signalfd "); exit(EXIT_FAILURE);
    }

    printf("My pid is: %d\n", getpid());

    for (;;) {
        struct signalfd_siginfo fdsi;
        ssize_t s;
        /*leggiamo dal file descriptor*/
        s = read(sfd, &fdsi, sizeof(fdsi));

        if (s != sizeof(fdsi))
        {
            perror("error read");
        }
        /*aggiorniamo il tempo*/
        update_time();

        int exist = 0;
        
        /*e' un segnale SIGUSRn*/
        if (fdsi.ssi_signo == SIGUSR1 || fdsi.ssi_signo == SIGUSR2)
        {
            printf("Got SIGUSR\n");
            /*apriamo un directory stream per vedere se gia' esiste*/
            DIR *dir = opendir(".");
            struct dirent *entry;
            char str[100];
            /*convertiamo il pid in stringa*/
            sprintf(str, "%d", fdsi.ssi_pid);
            while ((entry = readdir(dir)) != NULL)
            {
                if (strcmp(entry->d_name, str) == 0)
                {
                    exist = 1;
                    /*se esiste apre il file in append mode e ci scrive*/
                    int fd = open(entry->d_name, O_APPEND | O_RDWR);
                    if (fdsi.ssi_signo == SIGUSR1)
                    {
                        char buff[32];
                        sprintf(buff, "USR1 %s\n", buffer);
                        write(fd, buff, strlen(buff));
                    }
                    else
                    {
                        char buff[32];
                        sprintf(buff, "USR2 %s\n", buffer);
                        write(fd, buff, strlen(buff));
                    }
                    close(fd);   
                }    
            }
            /*se non esiste*/
            if (!exist)
            {
                char str[100];
                sprintf(str, "%d", fdsi.ssi_pid);
                /*crea un nuovo file e ci scrive*/
                int fd = open(str, O_CREAT | O_RDWR, 0600);
                if (fdsi.ssi_signo == SIGUSR1)
                {
                    char buff[32];
                    sprintf(buff, "USR1 %s\n", buffer);
                    write(fd, buff, strlen(buff));
                }
                else
                {
                    char buff[32];
                    sprintf(buff, "USR2 %s\n", buffer);
                    write(fd, buff, strlen(buff));
                }
                close(fd);   
            }    
        }
        else
        {
            printf("Read unexpected signal\n");
        }
    }
    close(sfd);
}
