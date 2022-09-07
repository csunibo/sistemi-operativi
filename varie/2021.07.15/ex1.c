/*
Rendete la vostra directory home inaccessibile ad altri utenti (sia in lettura che in esecuzione)
	sudo chmod 0700 ~/ oppure sudo chmod 0700 /home/matteo.manuelli

Si puo’ tenere una sola directory in /public che abbia come nome il vostro username e.g. “/public/giovanni.rossi” e che abbia permessi 700
	cd /public sudo mkdir matteo.manuelli sudo chmod 0700 ./matteo.manuelli
*/
/*
Sia dato questo programma hw.c:

#include <stdio.h>

int main(int argc, char*argv[]) {
	printf("hello world:");
	for(argv++, argv--; argc > 0; argv++, argc--) //se rimuovo "argv++, argv--" il risultato è lo stesso
		printf(" %s",*argv);
	printf("\n");
	return 0;
}

Il programma hw.c può essere compilato come libreria dinamica:
	gcc --shared -o hw.so hw.c

La libreria dinamica non è un eseguibile
	$ ./hw.so 1 2 3 4
	Segmentation fault

ma può essere caricata a tempo di esecuzione tramite dlopen. Scrivere un programma "lancia" in grado di eseguire il codice di hw.so
	$ ./lancia hw.so 1 2 3 4
	hello world: hw.so 1 2 3 4

(suggerimenti: dlopen non cerca nella directory corrente, occorre passare il path assoluto della libreria.
"main" in hw.so è una normale funzione: occorre cercare l'indirizzo della funzione main nella libreria
ed invocarla,)
*/
/*
Estendere l'esercizio 1. Il nuovo programma autolancia deve riconoscere se il primo parametro è una
libreria dinamica o un eseguibile gestendo entrambi i casi:
	gcc -o hw hw.c
	$ ./autolancia hw.so 1 2 3 4
	hello world: hw.so 1 2 3 4
	$ ./autolancia hw 1 2 3 4
	hello world: hw.so 1 2 3 4
*/
//gcc -o lancia ex1.c
#include <dlfcn.h> //man dlopen
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "davolib.h"
#include <sys/types.h>

int tryToExecute(char* path, char *argv[]){
	pid_t pid = fork(); //ad ogni thread verrà restituito il pid dell'altro partner
	if (pid == 0){ //al figlio viene restituito il pid del padre, ovvero 0
		return execv(path, argv);
	}
	else {
		int status;
		waitpid(pid, &status, 0); //aspetto che il figlio (con pid = pid) termini
		return WIFEXITED(status);
	}
}

void main(int argc, char const *argv[])
{
	if (argc < 2){
		fprintf(stderr, "usage: %s <file.so> ...\n", argv[0]);
		exit(1);
	}
	char *libraryName = argv[1];
	char buf[256]; getcwd(buf, 256); //se la path è più lunga di 256 char (compreso il terminating null byte) => ritorna NULL
	string_t *libraryPath = create_string(buf);
	//free(buf); Local variables are automatically freed when the function ends. You only need to free dynamically allocated memory (e.g malloc) as it's allocated on the heap
	append_string(libraryPath, "/");
	append_string(libraryPath, libraryName);

	/*
	inizialmente per completare il secondo punto, avevo pensato di salvare in un array tutti i token separati da un punto, e controllare
	se l'ultimo token fosse "so". Problema: non necessariamente le librerie dinamiche hanno l'estensione .so
	Poi ho pensato di usare "stat" (il quale retrieve information about the file pointed to by pathname) per controllare
	che l'argomento fornito sia un eseguibile tramite maschera di bit. Problema: è un eseguibile (si nota anche dall'esempio del prof...)
	https://stackoverflow.com/questions/13098620/using-stat-to-check-if-a-file-is-executable-in-c
	https://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html
	struct stat sb;
	printf("%s is%s executable.\n", argv[1], stat(argv[1], &sb) == 0 && sb.st_mode & S_IXUSR ? "" : " not");
	Soluzione: exec ritorna solo se riscontra un errore (il seg fault di hw.so) =>
	*/

	if (tryToExecute(libraryName, argv + 1)){
		//fprintf(stderr, "%s\n", dlerror());
		printf("\neseguibile\n");
	}
	else {

		void *libraryHandle = dlopen(libraryPath->data, RTLD_NOW);
		//i simboli delle funzioni vengono risolti solo quando le utime vengono chiamate
		if (libraryHandle == NULL){ //on error dlopen() returns NULL
			fprintf(stderr, "dlopen(): %s\n", dlerror()); //se si usa solo il nome della libreria, dirà che non la trova => path assoluta
			exit(1);
		}

		//function pointer to get the address of "main" function in loaded library
		int (*func)(int, char**) = dlsym(libraryHandle, "main");

		func(argc -1, argv + 1);
		//il contatore degli argomenti e gli argomenti non conteranno il nome del wrapper "lancia"

		dlclose(libraryHandle);
	}
}