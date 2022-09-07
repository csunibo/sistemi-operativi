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

/*per compilare usare opzione -lpthread*/

/*variabile condivisa tra i processi*/
int shared_variable;
/*fd usati per i semafori*/
int ok2read, ok2write;

static void *producer()
{
    /*impostiamo il seed del generatore di numeri random al tempo corrente*/
    srand(time(NULL));
    /*il valore di eventfd deve avere questo tipo*/
    uint64_t semvalue; //semvalue = semaforo

    while (1)
    {
        /*se il semaforo ha valore 0 si blocca in attesa che venga incrementato*/
        //left->right : il file descriptor riempe il nostro semaforo
        read(ok2write, &semvalue, sizeof(uint64_t));

        /*aggiorna la variabile*/
        shared_variable = rand();

        semvalue = 1;

        printf("Ok to read\n");

        /*da ok di lettura impostando ad 1 il semaforo*/
        write(ok2read, &semvalue, sizeof(uint64_t));

        /*dorme un po*/
        sleep(rand() % 3);

    }



}

static void *consumer()
{
    /*impostiamo il seed del generatore di numeri random al tempo corrente*/
    srand(time(NULL));

    uint64_t semvalue;

    while (1)
    {
        /*duale che per il producer*/
        read(ok2read, &semvalue, sizeof(uint64_t));

        printf("Read value is %d\n", shared_variable);

        semvalue = 1;

        write(ok2write, &semvalue, sizeof(uint64_t));

        sleep(rand() % 3);

    }
}


int main(int argc, char const *argv[])
{
    //si passano il valore...
    pthread_t producer_id, consumer_id;

    /*inizialmente non puo' leggere quindi si inizializza a 0*/
    ok2read = eventfd(0, EFD_SEMAPHORE);

    /*inizialmente puo' scrivere quindi si sinizalizza a 1*/
    ok2write = eventfd(1, EFD_SEMAPHORE);

    /*creazione processi producer, consumer*/
    pthread_create(&producer_id, NULL, &producer, NULL);

    pthread_create(&consumer_id, NULL, &consumer, NULL);

    /*aspettiamo che terminino*/
    pthread_join(consumer_id, NULL);

    pthread_join(producer_id, NULL);

    return 0;
}
