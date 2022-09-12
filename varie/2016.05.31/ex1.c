/* Scrivere un programma che preso come parametro a linea comando il path di una directory elenchi solamente I file */
/* che hanno un nome che ha come suffisso un numero (es. Prova.10). I file devono essere ordinati in ordine */
/* numerico. */
/* Esempio se la directory test contiene I file */
/* prova5, giovanni, aaa.1000, bb.2000, ccc.dd.500 */
/* l'output del programma deve essere: */
/* ccc.dd.500 */
/* aaa.1000 */
/* bb.2000 */
/* (in quanto 500 numericamente e' minore di 1000, prova5 non si considera: manca il punto prima del numero). */
//SBAGLIATO! LE FORK CHIAMATE SUPPORTANO LA CONCORRENZA MA NON GESTISCO SEPARATAMENTE LE DUE LISTE !!!AGGIUSTATO CON DUE INDICI!!!
/* /1* void loopftn (void) *1/ https://stackoverflow.com/questions/6812242/defining-and-iterating-through-array-of-strings-in-c */
/* { */
/*   char *numbers[] = {"One", "Two", "Three", ""}, **n; */

/*   n = numbers; */
/*   while (*n != "") { */
/*     printf ("%s\n",  *n++); */
/*   } */
/*   return; */
/* } */
/* Come nell'esercizio 1 occorre cercare in una directory (passata come parametro) I file che hanno come suffisso un */
/* numero. Nell'esercizio 2 I file sono eseguibili e l numero indica il numero di millisecondi da attendere a partire dalla */
/* attivazione del programma prima di attivarli. */
/* Nell'esempio dell'esercizio precedente occorre aspettare mezzo secondo e lanciare ccc.dd.500, poi a 1 secondo */
/* dall'attivazione (cioe' dopo approssimativamente ulteriori 0.5 secondi) si lancia aaa.1000 e allo scadere del */
/* secondo secondo bbb.2000. */
/* I file eseguibili nella directory vengono eseguiti in background (non si attende la fine dell'esecuzione per */
/* continuare). Quindi se due file hanno lo stesso prefisso numerico vengono eseguiti in modo concorrente. */
#include <linux/limits.h>
#include <string.h>
/* #include <pthread.h> */ //non posso utilizzare n thread poichè utilizzo exec
#include <strings.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <ctype.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define TRUE 1
#define FALSE 0
#define MAX_FILES 4096
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define HANDLE_ERROR(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)
//prima memorizzo n suffissi in un array di interi. Lo ordino (e scopro chi è il massimo). Dopodichè per ogni elemento dell'array ricontrollo gli elemnti della directory e in quel momento stampo quegli elementi della directory che hanno il suffisso uguale all'elemento dell'array. Complessità O(m*n). Il tempo corrente sarà il max tra il tempo corrente attuale e il tempo di attesa dell'iesimo processo. E l'iesimo processo aspetterà il tempo attuale meno il tempo vecchio

/* https://stackoverflow.com/questions/8115624/using-quick-sort-in-c-to-sort-in-reverse-direction-descending */
int comp(const void * a, const void * b)
{
	return *(int *)a - *(int *)b;
}

int not_already_in_array(int *array, int length, int elem){
	for (int i =0; i< length; i++){
		if (*(array + i) == elem) return FALSE;
	}
	return TRUE;
}
/* https://stackoverflow.com/questions/3035225/getting-file-extension-in-c-language */
char *get_filename_ext(const char *filename){
	char *e = strrchr (filename, '.');
	if (e == NULL)
		e = ""; // fast method, could also use &(filename[strlen(filename)]).
	return e;
}

/* https://stackoverflow.com/questions/14422775/how-to-check-a-given-string-contains-only-number-or-not-in-c#:~:text=You%20can%20use%20the%20isdigit,string%20for%20containing%20numbers%20only.&text=Subminiature%20stylistic%20sidenote%3A%20returns%20true,might%20not%20want%20this%20behavior. */
int digits_only(const char *s)
{
	while (*s) {
		if (isdigit(*s++) == 0) return FALSE;
	}
	return TRUE;
}

int main(int argc, char **argv){
	int suffixes[MAX_FILES]; INIT_ARRAY(suffixes);
	char **files = (char**)malloc(MAX_FILES*sizeof(char *));
	for (int j = 0; j< MAX_FILES; j++){
		files[j] = (char *)malloc(PATH_MAX*sizeof(char));
		bzero(files[j], PATH_MAX);
	}
	int n = 0; //indice di scorrimento array SUFFISSI
	int k = 0; //indice di scorrimento array FILES
	char input[PATH_MAX]; INIT_ARRAY(input);
	printf("Type in a dir: ");
	if (fgets(input, sizeof(input), stdin) == NULL){
		HANDLE_ERROR("fgets()1\n");
	}
	/* printf("Hai digitato %s", input); */
	/* https://stackoverflow.com/questions/33893359/no-such-file-or-directory-and-segmentation-fault-opening-a-file */
	strtok(input, "\n");
	DIR *dir = opendir(input);
	if (dir == NULL) HANDLE_ERROR("opendir()1\n");
	struct dirent *ent; bzero(ent, sizeof(struct dirent));
	while ((ent = readdir(dir)) != NULL){
		if ( (strcmp(".", ent->d_name) == 0) || (strcmp("..", ent->d_name) == 0) ){
			continue;
		}
		char buf[PATH_MAX]; INIT_ARRAY(buf);
		snprintf(buf, PATH_MAX, "%s", get_filename_ext(ent->d_name));
		/* printf("%s\n", buf); */
		//controllo che abbia un'estensione
		if ( (strcmp(buf, "") == 0) && (strcmp(buf, ".") == 0) ){
			continue;
		}
		if (digits_only(buf + 1)){ //include il punto
								   //TODO controllare che non sia già nella lista il numero =>
			/* fprintf(stderr, "%s contato\n", ent->d_name); */
			snprintf(files[k], PATH_MAX, "%s", ent->d_name);
			k++;
			if (not_already_in_array(suffixes, n, atoi(buf +1))){
				suffixes[n] = atoi(buf + 1);
				/* printf("suffixes[n] = %d\n", suffixes[n]); */
				n++; //conto solo i file che devo lanciare
			}
		}
	}
	/* https://stackoverflow.com/questions/59694101/qsort-creates-a-non-specific-number-of-zeros */ //=> NON sizeof(suffixes)/sizeof(*suffixes)
	qsort(suffixes, n, sizeof(*suffixes), comp);
	int current_time = 0, time_to_wait = 0; //init
											//per ogni suffisso controlliamo ogni file
	for (int i=0; i< n; i++){
		//bisogna convertire
		int time_to_wait = suffixes[i] - current_time;
		printf("Time to wait= %d\n", time_to_wait);
		for (int j=0; j< k; j++){ //itero lungo i file
			char tmp[PATH_MAX]; INIT_ARRAY(tmp);
			/* https://stackoverflow.com/questions/190229/where-is-the-itoa-function-in-linux */
			snprintf(tmp, PATH_MAX, "%d", suffixes[i]);
			/* printf("tmp = %s\n", tmp); */
			if(strcmp(get_filename_ext(files[j]) + 1, tmp) == 0){
				printf("%s\n", files[j]);
				pid_t pid = fork();
				if (pid == -1){ HANDLE_ERROR("\nfork()1"); }
				else if (pid == 0) {
					//figlio
					usleep(time_to_wait * 1000);
					char path_to_exe[PATH_MAX]; INIT_ARRAY(path_to_exe);
					snprintf(path_to_exe, PATH_MAX, "%s/%s", input, files[j]);
					execl(path_to_exe, path_to_exe, NULL);
					fprintf(stderr, "ERRORE\n");
				}
			}
		}
		if ( suffixes[n-1] == suffixes[i] ) {

			printf("Main thread is going to exit without waiting\n");
			exit(EXIT_SUCCESS);
		}
		usleep(time_to_wait * 1000);
		current_time+=time_to_wait;
	}
}
