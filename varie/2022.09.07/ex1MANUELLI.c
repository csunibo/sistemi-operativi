//e' quello corretto
//se digitiamo ./a.out echo ciao , finche' ci permette di creare processi p[rosegue
//https://github.com/cosimopp/prove-pratiche-so/blob/880fd3da264666d14e641f4ba48e5902a29e7415/2018.07.18/clientSig.c
#define EX 1 // funziona con ctrl-d
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define HANDLE_ERROR(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

int launch(char **argv){
	pid_t pid1 = fork(); //pid figlio
	if (pid1 == -1) HANDLE_ERROR("fork");
	else if (pid1 == 0) { //figlio
		return execvp(argv[0], argv);
		exit(EXIT_FAILURE);
	}
	pid_t timer = fork();
	if (timer == -1) HANDLE_ERROR("fork2");
	else if (timer == 0){
		sleep(1);
		exit(EXIT_SUCCESS);
	}
	int status;
	pid_t first_arrived = wait(&status);
	if (first_arrived == timer){
		// indichiamo di non ripetere piu' il loop
		//e aspettiamo l'esecuzione dell'altro ?
		wait(0);
		if (EX == 2) return 0;
		else if (EX == 1) return 1;
	}
	else {
	
		//il timer non ci serve piu'
		kill(timer, SIGKILL);
		fprintf(stderr, "exited, status=%d\n", WEXITSTATUS(status));
		if (WEXITSTATUS(status) == 0){
			return 1;
		}
		else {
			return 0;
		}
	}	
}
int main(int argc, char **argv){
	if (argc < 2){
		fprintf(stderr, "BADUSAGE\n");
		exit(EXIT_FAILURE);
	}
	
	while(launch(argv+1));
	exit(EXIT_SUCCESS);
}
