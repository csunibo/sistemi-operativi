#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char const *argv[]){
    if (argc < 2){
        fprintf(stderr, "\nBAD USAGE");
        exit(EXIT_FAILURE);
    }
    FILE *stream = fopen("/tmp/countpid", "r");
    if (stream == NULL) HANDLE_ERROR("\nfopen()");

    int itsPid = -1;
    if (fread(&itsPid, sizeof(int), 1, stream) == 0){
        HANDLE_ERROR("\nfread()");
    }
    fclose(stream);
    kill(itsPid, SIGUSR1);

    int pid = fork();
    if (pid == -1){HANDLE_ERROR("\nfork()");}
    else if (pid == 0) {
        /* https://stackoverflow.com/questions/10784943/using-exec-with-only-some-parameters-of-argv-using-c */
        /* char **params = (char**)malloc((argc)*sizeof(char*)); */
        /* for (int i=0; i<argc; i++){ */
        /*     sprintf(params[i], "%s", argv[i+1]); */
        /* } */
        /* params[argc] = NULL; */
        /* fprintf(stderr, "\nparam[0] = %s, param[1] = %s, ...param[argc] = %s", params[0], params[1], params[argc]); */
        execvp(argv[0], argv + 1);
    }
    else{
        int status;
        waitpid(pid, &status, 0);
        if ( WEXITSTATUS(status) ){
            printf("exited, status=%d\n", WEXITSTATUS(status));
        }
        kill(itsPid, SIGUSR2);
    }
    exit(EXIT_SUCCESS);
}
