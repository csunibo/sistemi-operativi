//telegram link: https://t.me/c/1449061520/1326
//dopo avvenuta discussione tra @LLibera e @rd235

/*
Prova Pratica di Laboratorio di Sistemi Operativi
23 gennaio 2023

Esercizio 1: Linguaggio C (obbligatorio) 20 punti
Scrivere un programma fifotext che:
* crei una named pipe (FIFO) al pathname indicato come primo (e unico) argomento.
* apra la named pipe in lettura
* stampi ogni riga di testo ricevuta
* se la named pipe viene chiusa la riapra
* se riceve la riga "FINE" termini cancellando la named pipe.
Esempio:
fifotext /tmp/ff
....
se in un altra shell si fornisce il comando: "echo ciao > /tmp/ff", fifotext stampa ciao e rimane in attesa
(questo esperimento si può provare più volte). Con il comando "echo FINE > /tmp/ff" fifotext termina.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if( argc < 2 ) {
		perror( "Bad usage! u have to specify pipe name\n" );
		return -1;
	}
	FILE *mypipe;
	size_t buflen = 0;
	char *buf = NULL;
	size_t len;
	// crea una named pipe (fifo) al path indicato come primo e unico argomento
	// mkfifo(<pathname>, <permission>)
	char *myname = argv[1];

	if( mkfifo(myname, 0666) < 0 ){
		perror( "Cannot create a pipe\n" );
	}
	while (1) { 
		//apra la named pipe in lettura
		mypipe = fopen( myname, "r" );

		while ( len = getline(&buf, &buflen, mypipe ) > 0 ) {
	    		printf( "Received: %s", buf );
			int exit = ! strcmp(buf, "FINE\n");
	    		if( exit ) {	//compare buf con "FINE"
				printf("fine!\n");
				unlink(myname);
				return 0;
			}
		}
		fclose(mypipe);
	}
	return 0;
}

