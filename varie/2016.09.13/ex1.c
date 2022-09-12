/* Scrivere un programma che mostri il funzionamento del file locking. */
/* In particolare il programma cplk avra’ due argomenti che I pathname di due file. Il contenuto del primo file deve */
/* essere copiato nel secondo usando la system call sendfile. */
/* Prima di iniziare la copia occorre usare la system call fcntl per bloccare il file di output (write lock). */
/* Completata la copia occorre chiamare nuovamente la fcntl per sbloccare il file. */
/* Inserire un ritardo (sleep) con valore casuale da 1 a 10 secondi sia prima sia dopo aver sbloccato il file e */
/* stampe di controllo che indichino quando il file viene bloccato e sbloccato. */
/* In questo modo lanciando piu’ esecuzioni concorrenti con output sullo stesso file deve apparire che l’accesso in */
/* scrittura e’ mutualmente esclusivo. */
/* Es. di output: */
/* $ cplk cplk.c out */
/* locked attende da 1 a 10 sec */
/* unlocked attende da 1 a 10 sec */
/* $ */
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <time.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define TRUE 1
#define FALSE 0
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

enum _PIPE {READ, WRITE};
/* https://www.informit.com/articles/article.aspx?p=23618&seqNum=4 */
int main (int argc, char* argv[]){
    if (argc != 3){
        fprintf(stderr, "\nBAD USAGE");
        exit(EXIT_SUCCESS);
    }
    int fd_read = open(argv[1], O_RDONLY);
    if (fd_read == -1) HANDLE_ERROR("\nfd_read");
    struct stat buf; bzero(&buf, sizeof(struct stat));
    if (fstat(fd_read, &buf) == -1) HANDLE_ERROR("\nfstat()");
    /* Open the output file for writing, with the same permissions as the source file. */
    int fd_write = open(argv[2], O_WRONLY | O_CREAT, buf.st_mode);
    if (fd_write == -1) HANDLE_ERROR("\nfd_write");

    struct flock lock; bzero(&lock, sizeof(struct flock));
    srand(time(NULL));
    int random_time = rand() % 11;
    printf("\nwaiting %d secs then lock", random_time);
    sleep(random_time);
    /* Initialize the flock structure. */
    printf("\nWaiting to get the lock");
    lock.l_type = F_WRLCK;
    fcntl (fd_write, F_SETLKW, &lock);
    printf("\nLocked!");

    if(  sendfile(fd_write, fd_read, NULL, buf.st_size) == -1) HANDLE_ERROR("\nsendfile()");
    random_time = rand() % 11;
    /* Release the lock. */
    lock.l_type = F_UNLCK;
    fcntl (fd_write, F_SETLKW, &lock);
    printf("\nUnlocked!");
    close(fd_read);
    close(fd_write);
    exit(EXIT_SUCCESS);

}
