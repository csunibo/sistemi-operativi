//exec execs exectuable, not bash scripts! Pipe operator is specific to bash
#include <unistd.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
int main(int argc, char **argv){
	int fd[2];
	if (pipe(fd) == -1) HANDLE_ERROR("pipe");
}
