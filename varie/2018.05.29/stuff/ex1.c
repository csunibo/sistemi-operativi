/* Fare un programma di confronto parallelo di file. */
/* pcmp file1 file2 */
/* non stampa nulla se i due file sono uguali altrimenti stampa "file1 file2 differ". pcmp deve creare due processi figli; */
/* mentre un processo controlia la prima meta’ del file, l'altro deve controllare l’altra meta’. */
/* Se un dei due processi figli trova una differenza, termina immediatamente segnalandolo al padre con opportuno */
/* valore di ritorno. In questo caso il padre deve terminare l'altro processo. */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#define TRUE 1
#define FALSE 0
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int file_size(char *pathname){
    struct stat buf; //bzero(buf, sizeof(*buf));
    /* fprintf(stderr , "sono qui"); */
    if (stat(pathname, &buf) == -1) handle_error("\nstat()");
    return (int)buf.st_size;

}
int main(int argc, char **argv){
    if (argc < 3){
        fprintf(stderr, "\nBAD USAGE");
        exit(EXIT_FAILURE);
    }
    /* fprintf(stderr, "OK1"); */
    int n=2; //grado di parallelismo
    int filesize = file_size(realpath(argv[1], NULL));
    /* if (filesize != file_size(realpath(argv[2], NULL))){ */
    /*     printf("\ndiff %s %s", argv[1], argv[2]); */
    /*     exit(EXIT_SUCCESS); */
    /* } */

    /* fprintf(stderr, "OK1"); */
    int fd1 = open(argv[1], O_RDONLY);
    int fd2 = open(argv[2], O_RDONLY);
    if ( (fd1 == -1) || (fd2 == -1) ) handle_error("\nopen()");

    FILE *stream1 = fdopen(fd1, "r");
    FILE *stream2 = fdopen(fd2, "r");
    if ((stream1 == NULL) || (stream2 == NULL)) handle_error("\nfdopen()");
    int offset = -1*( 512/n );

    size_t read1 = 1, read2 = 1;
    /* while((read = fread(buf1, sizeof(char), BUFFER_SIZE, src))){} */
    while (read1 && read2){

        char buf1[512]; INIT_ARRAY(buf1);
        char buf2[512]; INIT_ARRAY(buf2);


        //supponendo che con gli stram venga mantenuto un offset statico
        if ( ( read1 = fread(buf1, sizeof(char), sizeof(buf1), stream1) )<0 ) handle_error("\nread");
        if ((read2 = fread(buf2, sizeof(char), sizeof(buf1), stream2))<0) handle_error("\nread");

        if (read1 == read2 == 0) continue;
        /* fprintf(stderr, "OK1"); */
        /* if (read(fd1, buf1, sizeof(buf1))<=0) handle_error("\nread()"); */
        /* if (read(fd2, buf2, sizeof(buf2))<=0) handle_error("\nread()"); */
        /* fprintf(stderr, "OK1"); */

        char parts[2][n][sizeof(buf1)/n]; INIT_ARRAY(parts); //array diviso verticalmente

        /* printf("HANNO STESSA DIM"); */
        for (int i=0; i<n; i++){
            offset = i*( sizeof(buf1)/n );
            pid_t pid = fork();
            if (pid == -1){handle_error("\nfork()");}
            else if (pid == 0){
                /* fprintf(stderr, "ENTRATO IN FORK"); */
                memcpy(parts[1][i], buf1 + offset, sizeof(buf1)/n); //iesima parte del primo file
                memcpy(parts[2][i], buf2 + offset, sizeof(buf1)/n); //iesima parte del primo file
                for (int j=0; j<(sizeof(buf1)/n); j++){
                    printf("processo %d %c==%c?", i, parts[1][i][j], parts[2][i][j]);
                    if ( parts[1][i][j] != parts[2][i][j] ){
                        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAaAAAAAAAPROCESSO %d %c==%c?", i, parts[1][i][j], parts[2][i][j]);
                        return FALSE;
                    }
                }
                return TRUE;
            }
        }
        for (int k=0; k<n; k++){
            int wstatus;
            waitpid(-1, &wstatus, 0); // Store proc info into wstatus
            int return_value = WEXITSTATUS(wstatus);
            if (return_value == FALSE){
                printf("%s %s DIFFFFFFFFFFFFFFFF", argv[1], argv[2]);
                exit(EXIT_SUCCESS);
            }
        }
    }
    printf("OK!");
    fclose(stream1);
    fclose(stream2);
    exit(EXIT_SUCCESS);
}
