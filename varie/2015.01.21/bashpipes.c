/* gcc bashpipes.c execs.c -o filepipe */
/* Scrivere un programma C di nome filepipe che abbia come unico parametro il pathnae di un file di testo. */
/* Questo file contiene due comandi con I rispettivi parametri, uno per riga. */
/* Il programma deve mettere in esecuzione concorrente I due comandi in modo che l'output del primo venga fornito */
/* come input del secondo usando una pipe. */
/* Il programma deve terminare quando entrambi I comandi sono terminati. */
/* Esempio: se il file ffff contiene: */
/* ls -l */
/* tac */
/* il comando: */
/* filepipe ffff */
/* deve restituire lo stesso output del comando: */
/* ls -l | tac */

/* Il secondo esercizio estende il primo. Il file passato come parametro in questo caso ha un numero arbitario di righe */
/* e non solo due come nell'esercizio1. Ogni riga contiene un comando con I rispettivi parametri. Il programma deve */
/* attivare tutti I comandi del file in modo concorrente, e terminare quando tutti finiscono. */
/* L'output di tutti I comandi, tranne l'ultimo, deve essere fornito in input all'ultimo comando (quello nell'ultima riga */
/* significativa del file). */
/* Es: se il file contenesse: */
/* ls -l */
/* ls -l */
/* ls -l */
/* cat */
/* l'output dovrebbe essere la lista dei file della corrente directory 3 volte (nell'output puo' capitare che righe di */
/* comandi diversi vengano frapposte, l'accesso alla pipe e' conteso). */

#include <stdlib.h>
#include "execs.h"
#include <strings.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#define MAX_STR_LEN 128 //NULL included
#define MAX_STRS_NO 128
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define HANDLE_ERROR(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)
enum _PIPE {READ, WRITE};
void launch(char *buf, int *fds){
	char **argv=s2argv(buf);
	close(fds[READ]);
	dup2(fds[WRITE], STDOUT_FILENO);
	close(fds[WRITE]);
	execvp(argv[0], argv);
	s2argv_free(argv); HANDLE_ERROR("execvp");
}

int main (int argc, char **argv){
	if (argc != 2) {
		fprintf(stderr, "\nBAD USAGE");
		exit(EXIT_FAILURE);
	}
	int fds[2]; INIT_ARRAY(fds);
	if (pipe(fds) == -1) HANDLE_ERROR("pipe");
	char **cmds = (char **)malloc(MAX_STRS_NO*sizeof(char *));
	if (cmds == NULL) HANDLE_ERROR("malloc");
	for (int i=0; i< MAX_STRS_NO; i++) cmds[i] = (char *)malloc(MAX_STRS_NO*sizeof(char));
	FILE *stream = fopen(argv[1], "rb");
	if (stream == NULL) HANDLE_ERROR("fopen");
	char buf[MAX_STR_LEN]; INIT_ARRAY(buf);
	int no_elems = 0; //=> programs o run concurrently for i:=0; i< no_elems - 1; i++
	while (fgets(buf, MAX_STR_LEN, stream)){
		snprintf(cmds[no_elems], MAX_STR_LEN, "%s", buf);
		no_elems++; INIT_ARRAY(buf);
	}
	fclose(stream);
	for (int i=0; i<no_elems -1; i++){
		pid_t pid1 = fork();
		if (pid1 == -1) HANDLE_ERROR("fork");
		else if (pid1 == 0){
			launch(cmds[i], fds);
		}
	}
	pid_t pid2 = fork();
	if (pid2 == -1) HANDLE_ERROR("fork");
	else if (pid2 == 0){
		close(fds[WRITE]);
		dup2(fds[READ], STDIN_FILENO);
		close(fds[READ]);
		char **argv=s2argv(cmds[no_elems-1]);
		execvp(argv[0], argv);
		s2argv_free(argv); HANDLE_ERROR("execvp");
	}
	close(fds[READ]); close(fds[WRITE]); //altrimenti "grep" o "cat" in sempre in ascoltro finchè tutti i canali si chiudono
	/* for (int i=0; i<no_elems; i++){ */ //COMMENTANDO I PROCESSI CORRONO IN BACKGROUND
	/* 	int status; */
	/* 	wait(&status); */
	/* } */
	/* fprintf(stderr, "TUTTI HANNO FINITO\n"); */
	exit(EXIT_SUCCESS);
}
