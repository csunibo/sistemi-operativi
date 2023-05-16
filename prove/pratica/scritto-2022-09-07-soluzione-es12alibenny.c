/**
 * @file 2022-09-07
 * @author Alice Benatti (alice17bee@gmail.com)
 * @brief Prova pratica https://csunibo.github.io/sistemi-operativi/prove/pratica/scritto-2022-09-07-testo.pdf
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
  
// Esercizio 0
// dalla tua directory e home personale
// chmod 700 ../../../public/alice.benatti4/


/* Esercizio 1 *
	Scrivere un programma C rilancia che esegua
	- il programma indicato in argv[1] con i relativi parametri in argv[2] e seguenti
		es: 	rilancia tr a-z A-Z 
			esegue il comando tr con i relativi parametri
	Se il programma lanciato termina senza errori (con valore 0 e non per colpa di un segnale) => rilancia deve rieseguire il programma con i medesimi parametri.
*/
/* include necessari */
#include <stdio.h>		/* printf */
#include <stdlib.h> 	/* exit */
#include <string.h> 	/* strlen, strcpy, */ 
#include <sys/types.h>
#include <sys/wait.h>	/* vedi: man wait */
typedef unsigned int pid_t;	/* pid */

int rilancia (int argc, char* argv[]){
	// devo avere almeno 2 argomenti
	if (argc < 2) {
		printf("ERROR: inserisci almeno 2 argomenti nel comando");
		exit(1);
	}
	pid_t pid;
	int wait_status;
	int return_value = 1;
	char* argo[argc];
	argo[argc - 1] = NULL;

	for (int i = 0; i < (argc - 1); i++) {
		argo[i] = malloc(sizeof(char)* strlen(argv[i + 1]));
		strcpy(argo[i], argv[i + 1]);
	}

	do {
		switch (pid = fork()) {
			case -1:
				exit(1);
				break;
			case 0:
				printf("Esecuzione da processo pid = %d\n", getpid());
				execvp(argo[0], argo);
				break;
			default:
				if (waitpid(pid, &wait_status, 0) == -1)
					exit(1);
				return_value = WEXITSTATUS(wait_status);
				break;
		}
	} while (return_value == 0 && !(WIFSIGNALED(wait_status)));

	printf("Process terminato con errore");
	for (int i = 0; i < argc - 1; i++) {
		free(argo[i]);
	}
	return 0;
}

/** Esercizio 2
 * Si esegue il programma dell'esercizio 1 con:
 * rilancia cat /etc/hostname
 * il comando cat viene eseeguito ripetuttamente all'infinito.
 * Modificare il programma rilancia per fare in modo che 
 * - se l'esecuzione del programma dura meno di un secondo => non si proceda alla riattivazione.
*/
# include <time.h> 	/* time, clock */

int rilancia2 (int argc, char* argv[]){
	// devo avere almeno 2 argomenti
	if (argc < 2) {
		printf("ERROR: inserisci almeno 2 argomenti nel comando");
		exit(1);
	}
	pid_t pid;
	int wait_status;
	int return_value = 1;
	char* argo[argc];
	argo[argc - 1] = NULL;
	clock_t start, end;		/* clock */
	double exec_time;		/* clock */

	for (int i = 0; i < (argc - 1); i++) {
		argo[i] = malloc(sizeof(char)* strlen(argv[i + 1]));
		strcpy(argo[i], argv[i + 1]);
	}

	do {
		switch (pid = fork()) {
			case -1:
				exit(1);
				break;
			case 0:
				printf("Esecuzione da processo pid = %d\n", getpid());
				start = clock();
				execvp(argo[0], argo);
				end = clock();
				exec_time = (double)(end - start) / CLOCKS_PER_SEC;
				if (exec_time < 1){
					printf("Esecuzione troppo breve, non si procede alla riattivazione");
					exit(0);
				}
				break;
			default:
				if (waitpid(pid, &wait_status, 0) == -1)
					exit(1);
				return_value = WEXITSTATUS(wait_status);
				break;
		}
	} while (return_value == 0 && !(WIFSIGNALED(wait_status)));
	if ((return_value != 0) || WIFSIGNALED(wait_status))
  		printf("Process terminato con errore");
	for (int i = 0; i < argc - 1; i++) {
		free(argo[i]);
	}
	return 0;
}
