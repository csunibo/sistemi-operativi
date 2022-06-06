#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
	int status;
	int pfd[2];
	pipe(pfd);
	switch(fork()) {
		case 0:
			// limbo
			dup2(pfd[1], STDOUT_FILENO);
			close(pfd[0]);
			execvp(argv[1], argv+1);
			exit(1);

		default:
			break;

		case -1:
			exit(1);
	}
	switch(fork()) {
		case 0:
			// limbo
			dup2(pfd[0], STDIN_FILENO);
			close(pfd[1]);
			execlp("sort", "sort", NULL);
			exit(1);

		default:
			close(pfd[0]);
			close(pfd[1]);
			wait(&status);
			wait(&status);
			break;

		case -1:
			exit(1);
	}
}
