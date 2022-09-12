/*
Usando il metodo inotify implementare il programma dircat.
dircat ha 2 parametri: il pathname di una directory (vuota) D e di un file F.
Ogni volta che un file viene messo in D il programma dircat aggiunge a F una riga di testata
contenente il nome del file e ne copia il contenuto in F. Finita la copia il file che era stato messo in D
viene cancellato (la directory D torna vuota).
(per fare gli esperimenti si consiglia di preparare i file di prova in un'altra directory e copiarli in D).
*/
/*
completare dircat. Se il file aggiunto a D è un eseguibile dircat deve inserire in F dopo la riga di testata
l'output dell'esecuzione del nuovo file non già il suo contenuto. Completata l'esecuzione il file
eseguibile deve venir cancellato come nell'esercizio 1.

Commento di Matteo: riguardo l'ultima traccia, da terminale funziona, se fatto manualmente si sganghera tutto (ex non viene più mostrato
output sulla console ma viene scritto sul log)
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <unistd.h>
#include "davolib.h"
#include <errno.h>

void
main (int argc, char const *argv[])
{
  if (argc != 3)
    {
      fprintf (stderr, "usage: %s EMPTYDIRPATH LOGFILEPATH\n", argv[0]);
      exit (EXIT_FAILURE);
    }
  /* Create the file descriptor for accessing the inotify API */
  //inotify file handle should not block read operations. 
  /*
     IN_CLOEXEC
     close-on-exec flag for the file descriptor, which causes the file descriptor to be automatically (and atomically) closed when any of the exec-family functions succeed.
     IN_NONBLOCK
     le funzioni NON rimangono in attesa finché non vi sono dati da leggere disponibili sul descrittore
     Altre flag e mask in man 2 open
     http://www.ce.uniroma2.it/courses/iw08/lucidi/socket_4_2pp.pdf
   */
  int fd = inotify_init1 (IN_NONBLOCK);	//creates an inotify instance and returns a file descriptor referring to the inotify instance (everything is a file)
  FILE *logFile = fopen (argv[2], "a");
  if (logFile == NULL)
    {
      perror ("fopen");
      return;
    }
  if (!inotify_add_watch (fd, argv[1], IN_CREATE))
    {
      perror ("inotify_add_watch");
      return;
    }				//controllo solo una directory => non c'è necessità di una watchList.
  //inoltre questa libreria freedda tutte le risorse associate all'istanza non appena la chiudiamo con close(fd).
  //La lista wd conterrebbe ID per identificare directory (tramite wd[i] == event->wd) quando si verifica un evento
  //L'identificativo è un descrittore associato all'evento di ogni directory: wd[i] = inotify_add_watch(fd, argv[i], IN_OPEN | IN_CLOSE); if (wd[i] == event->wd); //è avvenuto per quella directory

  //variabili usate per salvare e leggere l'evento (dovremo fare un casting per interpretare cosa abbiamo salvato)
  char buf[4096];
  init_array (buf);		//memsettare array!
  size_t len;
  struct inotify_event *event;
  string_t *newFilePath = create_string ("");	//la creo qui così non devo freeddarla ma l'aggiorno di volta in volta. stringa vuota

  while (1)
    {
      len = read (fd, buf, sizeof (buf));
      if (len == -1 && errno != EAGAIN)
	{
	  perror ("read");
	  exit (EXIT_FAILURE);
	}
      //If the nonblocking read() found no events to read, then it returns -1 with errno set to EAGAIN. EAGAIN = Resource temporarily unavailable
      if (len == -1 && errno == EAGAIN)
	continue;		//found no events to read

      /* Loop over all events in the buffer */
      for (char *ptr = buf; ptr < buf + len;
	   ptr += sizeof (struct inotify_event) + event->len)
	{

	  event = (struct inotify_event *) ptr;

	  if (event->mask & IN_ISDIR || !event->len)
	    continue;		//o directory o file vuoto (!0 = 1)

	  //scrivo il nome del file appena creato
	  if (!fprintf (logFile, "\n%s: ", event->name))
	    {
	      perror ("fprintf");
	      return;
	    }

	  fflush (logFile);	//La funzione fflush() forza la scrittura dei dati bufferizzati sullo stream tramite la funzione di basso livello write() => lo scaric
	  //senza di esso il programma non funzionava (scriveva solo il contenuto senza il nome e rimaneva bloccato)

	  //creo prima la stringa per individuare il file creato
	  update_string (newFilePath, argv[1]);
	  //append_string(newFilePath, "/");
	  append_string (newFilePath, event->name);

	  //check if new created file is whether an executable or not (tramite maschera bit di stat)
	  //https://stackoverflow.com/questions/13098620/using-stat-to-check-if-a-file-is-executable-in-c
	  //https://linux.die.net/man/2/stat
	  //http://manpages.ubuntu.com/manpages/trusty/man2/inotify_init.2.html
	  struct stat sb;
	  //no error && use S_IXUSR (S_IEXEC is an old synonym of S_IXUSR) to check if you have execute permission. Bitwise AND operator (&) checks whether the bits of S_IXUSR are set or not.
	  if (stat (newFilePath->data, &sb) == 0 && sb.st_mode & S_IXUSR)
	    {
	      /*executable */
	      char output[64];
	      init_array (output);

	      const char *args[2] = { newFilePath->data, NULL };

	      getOutput (args[0], args, output, sizeof (output));

	      printf ("%s\n", output);
	      fputs (output, logFile);

	      fflush (logFile);

	    }
	  else
	    {
	      appendFileToFile (newFilePath->data, argv[2]);
	    }

	  if (unlink (newFilePath->data))
	    {
	      perror ("unlink");
	      exit (EXIT_FAILURE);
	    }
	}
    }
}
