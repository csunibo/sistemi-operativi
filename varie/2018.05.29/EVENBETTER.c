/* Fare un programma di confronto parallelo di file. */
/* pcmp file1 file2 */
/* non stampa nulla se i due file sono uguali altrimenti stampa "file1 file2 differ". pcmp deve creare due processi figli; */
/* mentre un processo controlia la prima meta’ del file, l'altro deve controllare l’altra meta’. */
/* Se un dei due processi figli trova una differenza, termina immediatamente segnalandolo al padre con opportuno */
/* valore di ritorno. In questo caso il padre deve terminare l'altro processo. */
/* usando “getopt” consentire di scegliere il grado di parallelismo voluto: */
/*  pcmp -j 8 file1 file2 */
/* deve creare 8 processi, ogni processo controlla 1/8 del file. */
/* #include <bits/getopt_core.h> */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define TRUE 1
#define FALSE 0
#define BUFSIZE 512
#define FILES_TO_CMP 2
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char **argv){
    if (argc < 3){
        fprintf(stderr, "BAD USAGE");
        exit(EXIT_FAILURE);
    }

    int opt = getopt(argc, argv, "j:");
    if (opt == -1){
        fprintf(stderr, "BAD USAGE");
        exit(EXIT_FAILURE);
    }

    const int N = atoi(optarg); //grado di parallelismo
    FILE *streams[FILES_TO_CMP]; INIT_ARRAY(streams);
    long sizes[FILES_TO_CMP]; INIT_ARRAY(sizes);

    for (int i=0; i<FILES_TO_CMP; i++){
        streams[i] = fopen(argv[i+ 3], "r");
        if (streams[i] == NULL) HANDLE_ERROR("\nfopen");
        fseek(streams[i], 0, SEEK_SET);
        fseek(streams[i], 0, SEEK_END);
        sizes[i] = ftell(streams[i]);
        /* printf("\nsize dell'%desimo è %ld", i, sizes[i]); */
        rewind(streams[i]);
        fclose(streams[i]);
    }

    for(int i= FILES_TO_CMP -1; i> 0; i--){
        if (sizes[i] != sizes[i-1]){
            fprintf(stdout, "different since different size");
            exit(EXIT_SUCCESS);
        }
    }

    const long OFFSET = (*sizes)/N;
    pid_t pids[N]; INIT_ARRAY(pids);
    for (int n=0; n<N; n++){
        pid_t pid = fork();
        if (pid == -1){HANDLE_ERROR("\nfork");}
        else if (pid == 0){
            FILE *streams[FILES_TO_CMP]; INIT_ARRAY(streams);
            for (int j=0; j<FILES_TO_CMP; j++) {
                streams[j] = fopen(argv[j + 3], "r");
                if (streams[j] == NULL) HANDLE_ERROR("\nfopen");
            }
            long pos = MIN(n*OFFSET, *sizes); //perchè potrebbe non essere un multiplo di BUFSIZE
            /* fprintf(stdout, "\nsize: %ld", *sizes); */
            while (pos < MIN(*sizes, (n+1)*OFFSET)){
                //their private descriptor with their cursor position (DAVVERO NECESSARIO?)
                /* printf("\ncurrent position with proc %d: %ld", n, pos); */
                char bufs[N][BUFSIZE]; INIT_ARRAY(bufs);
                for (int j=0; j<FILES_TO_CMP; j++){
                    //FUCK OFF SEEKS! YOU DO NOT WORK!
                    /* fseek(streams[j], 0, SEEK_SET); */
                    /* fseek(streams[j], 0, MIN(pos, MIN((n+1)*OFFSET, *sizes))); */
                    long __pos = MIN(pos, MIN( (n+1)*OFFSET, *sizes ));
                    fsetpos(streams[j], (fpos_t *)&__pos);
                    if (fread(bufs[j], sizeof(char), BUFSIZE, streams[j]) < 0) HANDLE_ERROR("\nfread");
                }
                for (int j=FILES_TO_CMP-1; j>0; j--){
                    for(int k=0; k<BUFSIZE; k++){
                        /* printf("\n%c ==%c?", bufs[j][k], bufs[j-1][k]); */
                        if ( bufs[j][k] != bufs[j-1][k] ){
                            fprintf(stdout, "different buffers");
                            return FALSE;
                        }
                    }
                }
                pos+=BUFSIZE;
            }
            for (int j=0; j< FILES_TO_CMP; j++){
                fclose(streams[j]);
            }
            return TRUE;
        }
        else {
            pids[n] = pid;
        }
    }
    for(int i=0; i<N; i++){
        int wstatus;
        waitpid(-1, &wstatus, 0);
        if (WEXITSTATUS(wstatus) == FALSE){
            fprintf(stdout, "\ncatched the diff");
            for (int j=0; j<N; j++){
                /* kill(pids[j], SIGKILL); */
            }
            /* exit(EXIT_SUCCESS); */
        }
    }
    fprintf(stdout, "\nSAME CONTENT!");
    exit(EXIT_SUCCESS);
}
/* https://stackoverflow.com/questions/4408719/find-the-next-occurrence-of-a-variable-in-vim */
