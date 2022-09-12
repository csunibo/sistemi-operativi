/*
Creare una directory chiamata exec. Scrivere un programma execname che se viene aggiunto un file
nela directory exec interpreti il nome del file come comando con parametri, lo esegua e cancelli il file.
es: sopo aver lanciato execname:
 execname exec
a seguito di questo comando:
 touch 'exec/echo ciao mare'
il programma stampa:
 ciao mare
(consiglio, usare inotify)
*/
/*
modificare il programma dell'esercizio 1 per fare in modo che execname2 scriva l'output
dell'esecuzione nel file invece che cancellarlo.
Nell'esempio precedente il programma execname2 non emette alcun output ma il comando
 cat 'exec/echo ciao mare'
produce come risultato:
 ciao mare
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include "davolib.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

void main(int argc, char const *argv[])
{
	if (argc != 2){
    	fprintf (stderr, "usage: %s exec\n", argv[0]);
    	exit (EXIT_FAILURE);
  	}
	mkdir(argv[1], 0755); //argv[1] should be exec
	chdir(argv[1]); //change working directory
	int fd = inotify_init1(IN_NONBLOCK);
   	if (fd == -1){
    	perror("inotify_init1");
    	exit(EXIT_FAILURE);
   	}

   	if (!inotify_add_watch (fd, ".", IN_CREATE)){
    	perror ("inotify_add_watch");
    	exit(EXIT_FAILURE);
  	}

  	//variabili usate per salvare e leggere l'evento (dovremo fare un casting per interpretare cosa abbiamo salvato)
  	char buf[4096]; //per leggere l'evento
  	init_array(buf);		//memsettare array!
  	char output[512]; //per leggere l'output del file eseguito
  	init_array(output);		//memsettare array!
  	size_t len;
  	struct inotify_event *event;
  	string_t *pathBin = create_string("/bin/");
  	string_t *createdFile = create_string("./");

  	while (1){
		len = read (fd, buf, sizeof (buf));
	    if (len == -1 && errno != EAGAIN){
		  	perror ("read");
		  	exit (EXIT_FAILURE);
		}
	    //If the nonblocking read() found no events to read, then it returns -1 with errno set to EAGAIN. EAGAIN = Resource temporarily unavailable
	    if (len == -1 && errno == EAGAIN) continue;		//found no events to read

	    /* Loop over all events in the buffer */
	    for (char *ptr = buf; ptr < buf + len; ptr += sizeof (struct inotify_event) + event->len){

		  	event = (struct inotify_event *) ptr;

		  	if (event->mask & IN_ISDIR || !event->len) continue; //o directory o letto niente

		  	//ripristino stringa per ogni evento trovato, e buffer usato per leggere output
		  	update_string(pathBin, "/bin/");
		  	init_array(output);
		  	update_string(createdFile, "./");
		  	
		  	append_string(createdFile, event->name);
		  	printf("\nfile creato: %s\n", createdFile->data);

		  	int numeroToken = substring_occurrences(event->name, " ") + 1; //"ciao mondo" => due token
 			char *token[numeroToken + 1]; //+1 perchè NULL terminated
 			split_string(event->name, " ", token);

 			append_string(pathBin, token[0]);
 			getOutput (pathBin->data, token, output, sizeof(output));
 			
 			FILE *logFile = fopen (createdFile->data, "a"); //prima di eliminarlo, chiudi. Siamo nella directory exec => posso usare direttamente il suo nome
  			if (logFile == NULL){
    			perror ("fopen");
    			exit (EXIT_FAILURE);
    		}

	  		printf ("%s\n", output);
	      	fputs (output, logFile);
	  		fflush (logFile);
	  		//prima di eliminarlo, pause un attimo e controllo che l'output sia stato scritto anche nel file appena creato
	  		_pause();

	  		fclose(logFile);
	  		if (unlink(createdFile->data)){
	      		perror ("unlink");
	      		exit (EXIT_FAILURE);
	    	}
		}
	}  	
}