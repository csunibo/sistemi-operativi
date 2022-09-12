/* Scrivere due programmi C, tx e rx: tx deve trasferire a rx stringhe di max 8 caratteri usando i valori */
/* assegnati ai segnali (il parametro value della funzione sigqueue). */
/* Il programma rx deve per prima cosa stampare il proprio pid e attendere segnali. */
/* ill programma tx ha due parametri, il pid did rx e il messaggio. */
/* es: tx 22255 sigmsg */
/* (posto che rx sia in esecuzione con pid 22255, rx deve stampare sigmsg). */

/* Estendere i programmi dell'es.1 per consentire il trasferimento di stringhe di lunghezza arbitraria */
/* (iterando il procedimento 8 caratteri alla volta). */

/* https://man7.org/linux/man-pages/man3/sigqueue.3.html */
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int64_t char8_to_int64(char *s){
	return *((int64_t*)s);
}


int main(int argc, char* argv[]){
	if(argc != 3){
		printf("ERR! Usage: %s PID 8_CHAR_MSG\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	pid_t pid = (pid_t)atoi(argv[1]);
	int64_t msg_conv = char8_to_int64(argv[2]); 
	
	union sigval value;
	value.sival_ptr = (void*)msg_conv;
	int res = sigqueue(pid,SIGUSR1,value);
	//printf("Trying to send %s which is %ld to %ld\n",argv[2], msg_conv, pid);
	if(res != 0){
		printf("ERR ON SIGQUEUE!\n");
		exit(EXIT_FAILURE);
	}
	
	
	return 0;
}
