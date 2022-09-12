/* Esercizio 1: Linguaggio C (obbligatorio): (20 punti) */
/* Scrivere un programma che preso come parametro un file contenente un elenco di comandi (con I relativi parametri) li attivi */
/* tutti in esecuzione concorrente e rimanga in attesa. Quando il primo termina, vengono terminati (con segnale SIGTERM) tutti */
/* gli altri. (consiglio: puo' essere utile usare la mia libreria s2argv-execs) */
/* esempio: */
/* wta commands */
/* il file commands contiene: */
/* ./ttest 40 */
/* ./ttest 10 */
/* ./ttest 20 */
/* lo script ./ttest contiene: */
/* #!/bin/bash */
/* echo waiting for $1 seconds */
/* sleep $1 */
/* echo done $i */
/* l'ouptut sara': */
/* waiting for 40 seconds */
/* waiting for 10 seconds */
/* waiting for 20 seconds */
/* done 10 */
/* e poi basta perche' gli altri processi verranno terminati. */

/* Completare wta in modo che l'output di ogni comando venga salvato separatemente e solo l'output del processo terminato per */
/* primo venga mostrato. */
/* Nell'esempio di prima l'output dovra' essere: */
/* waiting for 10 seconds */
/* done 10 */

/* https://stackoverflow.com/questions/955962/how-to-buffer-stdout-in-memory-and-write-it-from-a-dedicated-thread */
		 //idea: creare pipe globali. Associare ad ogni pid un numero. Quando ricevo una wait, seleziono da quale pipe leggere
		 //riguardo l'ex1, basta far eseguire gli altri programmi ed aspettare una wait. No dup2 required in ex1
#include <sys/types.h>
#define MAX_STR_LEN 128 //NULL included
#define MAX_STRS_NO 128
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>
enum _PIPE {READ, WRITE};
#include <unistd.h>
#include "execs.h"
#include <fcntl.h>
#include <sys/epoll.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define HANDLE_ERROR(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)
void handle_error(int fds[][2], int i, char *msg){
	for (int j = 0; j<i; j++){
		for (int k=0; k<2; k++) close(fds[j][k]);
	}
	HANDLE_ERROR(msg);
}

int fds[MAX_STRS_NO][2];

void launch(char *buf, int *fds){
	char **argv=s2argv(buf);
	close(fds[READ]);
	dup2(fds[WRITE], STDOUT_FILENO);
	close(fds[WRITE]);
	execvp(argv[0], argv);
	s2argv_free(argv); HANDLE_ERROR("execvp");
}
int main(int argc, char **argv){
	if (argc != 2){
		fprintf(stderr, "\nBAD USAGE");
		exit(EXIT_FAILURE);
	}

	char **cmds = (char **)malloc(MAX_STRS_NO*sizeof(char *));
	if (cmds == NULL) HANDLE_ERROR("malloc");
	for (int i=0; i< MAX_STRS_NO; i++) cmds[i] = (char *)calloc(MAX_STRS_NO, sizeof(char)); //already inited
	FILE *stream = fopen(argv[1], "rb");
	if (stream == NULL) HANDLE_ERROR("fopen");
	char buf[MAX_STR_LEN]; INIT_ARRAY(buf);
	int no_cmds = 0; //in order to know how many pipes we gotta create
	while (fgets(buf, MAX_STR_LEN, stream)){
		snprintf(cmds[no_cmds], MAX_STR_LEN, "%s", buf);
		no_cmds++; INIT_ARRAY(buf);
	}
	fclose(stream);
	int arr[no_cmds]; //associazione indice pipe - pid: quando la wait si sblocca, controllo tutte le associazioni
	for (int i=0; i<no_cmds; i++){
		if (pipe(fds[i]) == -1){
			handle_error(fds, i, "pipe");
		}

		pid_t pid = fork();
		if (pid == 0){
			launch(cmds[i], fds[i]); //gli forniamo il comando da eseguire e la pipe
		}
		else if (pid != -1){
			arr[i] = pid; //pipe iesima ha pid pid
		}
		free( cmds[i] ) ;
		cmds[i] = NULL;
	}
	free(cmds);
	cmds= NULL;

	int wstatus;
	pid_t pid = wait(&wstatus);
	for (int i=0; i<no_cmds; i++){
		if (arr[i] == (int) pid){
			char tmp[4096]; INIT_ARRAY(tmp);
			if (read(fds[i][READ], tmp, sizeof(tmp)) == -1) HANDLE_ERROR("read");
			fprintf(stderr, "%s", tmp);
		}
		else {
			kill(arr[i], SIGTERM);
		}
		for (int k=0; k<2; k++) close(fds[i][k]);
	}
	exit(EXIT_SUCCESS);
}
