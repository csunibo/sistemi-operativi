#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
	int status;
	int fd = open("/tmp/out", O_CREAT|O_WRONLY|O_TRUNC);
	switch(fork()) {
		case 0:
			// limbo
			dup2(fd, STDOUT_FILENO);
			execvp(argv[1], argv+1);
			exit(1);

		default:
			wait(&status);

		case -1:
			exit(1);
	}
}
