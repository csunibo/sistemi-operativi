#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char * argv[]){
	pid_t pid;
	printf("%d before\n", getpid());
	if (pid = fork()) {
		int ws;
		printf("%d something is true\n", getpid());
		printf(" new proc is %d\n", pid);
		if (waitpid(42, &ws, 0) < 0) {
			printf(" waitpid ERROR %s\n", strerror(errno));
		} else if (WIFSIGNALED(ws))
			printf(" killed %d\n", WTERMSIG(ws));
		else
			printf(" wait ok %d\n", WEXITSTATUS(ws));
	} else {
		int *kaboom = NULL;
		sleep(1);
		printf("%d something is false\n", getpid());
		printf(" parent proc is %d\n", getppid());
		*kaboom = 0;
		_exit(42);
	}
}
