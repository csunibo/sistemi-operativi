#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
int main(int argc, char *argv[]){
	printf("%d\n", getpid());
	int status;
	switch( fork()){
	case 0:
                /* smart point */
		execvp(argv[1], argv+1);
        	perror("exec");
        	return 0;
	default:
		wait(&status);
		printf("done!\n");
		break;
	case -1:
		perror("fork");
		exit(-1);
	}
}
