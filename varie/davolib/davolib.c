#include <stdio.h>
#include <linux/limits.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <limits.h> /* PATH_MAX */

enum PIPES {READ, WRITE};

#define init_array(T) ( memset((T), 0, sizeof((T))) )

char *appendStr(char *base, const char *toAppend, size_t n){
    return strncat(base, toAppend, n);
}

int updateStr(char *toCopyInto, const char *toBeCopied, size_t lenToBeCopied){
    memset(toCopyInto, '\0', sizeof(*toCopyInto));
    return (strlen(strncpy(toCopyInto, toBeCopied, lenToBeCopied)) == strlen(toBeCopied) ? 1:0);
}


//we deal with strings! like JS ;) MA RICORDA: s->data E' UN PUNTATORE => QUANDO ASSEGNI STRNCPY!
typedef struct string {
	size_t length;
	char *data;
} string_t;


/*check -1 error*/
void cerror(int exp, const char *msg){
	if (exp == -1){
		perror(msg);
		exit(EXIT_FAILURE);
	}
}

//Salva dentro un buffer il risultato dell'esecuzione di un eseguibile
/*
esempio:
const char *args[3] = {"/bin/echo", "ciao", NULL};
char output[60];
init_array(output);
getOutput(args[0], args, output, 60);
printf("ottenudto: %s\n", output);
*/
//può eliminare file e eseguire comandi che non hanno input (clear funziona bene, cd ha un comportamento strano(copia output precedente, come se dovesse caricare una nuova pagina con esso presente))
//"exec loads an executable file and replaces the current program image with it. As you rightly noted, cd is not an executable file, but rather a shell builtin. So the executable that you want to run is the shell itself"
//https://stackoverflow.com/questions/9859903/using-the-exec-family-to-run-the-cd-command
int getOutput(const char *filePath, char * const* argv, char *buf, int bufSize){ //supposing buffer has allocated itself enough space
	int pipefd[2]; // Used to store two ends of pipe

	if (pipe(pipefd) == -1){
		perror("pipe");
		return 1;
	}

	pid_t pid = fork();
	if (pid == -1){
		/* Failure */
		perror("fork");
		return 1;
	} else if (pid == 0){
		/* Child */
		close(pipefd[READ]); // Close reading end of pipe. Figlio scrivo

		dup2(pipefd[WRITE], STDOUT_FILENO); //redireziono traffico 
		dup2(pipefd[WRITE], STDERR_FILENO);
		close(pipefd[1]); //non più utilizzato

		return execv(filePath, argv); //nel primo argomento oltre al nome dev'essere specificata la sua path
									  //Control never returns to the original program unless there is an exec() error
	} else {
		/* Parent */
		close(pipefd[WRITE]); // Close writing end of pipe. Padre legge
		wait(NULL); /* Wait for child */
		if (read(pipefd[READ], buf, bufSize) == -1){
			perror("read");
			return 1;
		}
		/*
		   fgets reads a line -- i.e. it will stop at a newline.
		   fread reads raw data -- it will stop after a specified (or default) number of bytes, independently of any newline that might or might not be present.
		   */
		/*clean up*/
		//close(pipefd[READ]); secondo me se eseguiamo questa istruzione dà errore perchè aspetta di leggere altri caratteri poichè non ne ha letti 60 (up)
		return 0;
	}
}

int getFileSize(const char *filePath){

	// opening the file in read mode
    FILE *fp = fopen(filePath, "r");
  
    // checking if the file exist or not
    if (fp == NULL) {
        return 1;
    }
  	
  	//get current position
	long pos = ftell(fp);

    fseek(fp, 0L, SEEK_END);
  
    // calculating the size of the file
    long size = ftell(fp);
  
    //ripristino - fp temporaneo => ridondante
    fseek(fp, 0L, (int)pos);

	 // closing the file
    fclose(fp);

	return (int)size;
}

void appendFileToFile(const char *toAppendPath, const char *dstPath){//append, non sovrascrive

	FILE *src = fopen(toAppendPath, "r");
	FILE *dst = fopen(dstPath, "a");

	if (src == NULL || dst == NULL){
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	//get file size
	fseek(src, 0L, SEEK_END);
	// calculating the size of the file
	int size = (int) ftell(src);
	//ripristino per leggere tutto il file
	fseek(src, 0L, SEEK_SET);

	char buf[size + 1]; //buffer temporaneo di appoggio

	while(fread(buf, sizeof(char), (size_t)size, src)){

		if(fwrite(buf, sizeof(char), (size_t)size, dst) != (size_t)size){
			perror("fwrite");
			exit(EXIT_FAILURE);
		}
	}

	fflush(dst);

	fclose(src);
	fclose(dst);
}
void writeIntoFile(const char *filePath, char *data){}

void _pause(){//aggiunto prefisso poichè anche unistd ha pause()
	printf("\nPress any key to continue . . .");
	getchar();
}

//https://stackoverflow.com/questions/14992772/modifying-string-literal-passed-in-as-a-function
//alla stringa anonima passata come argomento, la facciamo puntare ad una creata dentro alla funzione. Non uso memcpy o strcpy per paura di seg fault (no spazio allocatole)
//cos' facendo, non perdo spazio della vecchia stringa poichè questa è vuota!
//VECCHIO e SBAGLIATO SOTTO
//fornito un puntatore ad un buffer vuoto mai inizializzato, gli alloca lo spazio necessario per contenere l'intero contenuto di un file, e restituisce la sua dimensione
int readWholeFile(const char *filePath, char *buf){
	/* declare a file pointer and open an existing file for reading*/
	FILE *fp = fopen(filePath, "r"); //gli fp creati sono istanze separarate le une dalle altre => non c'è bisogno di nessun ripristino della posizione vecchia
	/* quit if the file does not exist */
	if(fp == NULL){
		return 1;
	}
	//The new position, measured in bytes, is obtained by adding offset bytes (0 long) to the position specified by whence (from what place or source.)
	fseek(fp, 0L, SEEK_END);//in order to get size

	//The ftell() function obtains the current value of the file position indicator for the stream pointed to by stream.
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET); //necessario altrimenti non leggeva
	/*
	   On files that support seeking, the read operation commences at the file offset, and the file offset is incremented by the number of bytes read.
	   If the file offset is at or past the end of file, no bytes are read, and read() returns zero.
	   */

	//alloco spazio temporaneo per il buffer
	/* char *temp = malloc(size*sizeof(char) +1); //non ritornerà lunghezza ma dimensione */
	/* temp[sizeof(temp)-1] = '\0'; */

	/* copy all the text into the buffer */
	fread(buf, sizeof(char), (size_t)size, fp); //The function fread() reads size items of data, each sizeof(char) bytes long,
												 //from the stream pointed to by stream, storing them at the location given by buf.
												 //printf("\n%s\n", temp);
	fclose(fp);

	/* *buf = temp; */

	/* free(temp); */
	return (int)(size + 1); //incluso il byte di parità

}

//https://www.programiz.com/dsa/avl-tree *((int*)void_PTR) //il file letto come pointer di intero viene deferenziato
//inizializzare array di stringhe => inizializza ogni stringa?

//every EDIT on data buffer should be carried out by davolib API in order to have up-to-date length
//aggiorna il contenuto di una stringa
void update_string(string_t* s, const char *newStr){
	size_t len;
	if (newStr != NULL){
		len = strlen(newStr);
	} else {
		len = 0;
	}
	//liberiamo la precedente
	free(s->data);
	s->data = (char *)malloc(len + 1); //Because of NULL terminator
	if (newStr != NULL){
		memcpy(s->data, newStr, len);
	}
	s->data[len] = '\0'; //memory returned by malloc isn't initialized to 0. Setting the first byte of new string to zero ensures that it looks like an empty string
	s->length = len;
}

//crea un'istanza di stringa. Fai finta che sia Python
string_t* create_string(const char *str){
	string_t* s = malloc(sizeof(string_t)); //It returns a POINTER to the reserved space
	size_t len;
	if (str != NULL){
		len = strlen(str);
	} else {
		len = 0;
	}
	s->data = (char *)malloc(len + 1); //Because of NULL terminator
	if (str != NULL){
		memcpy(s->data, str, len);
	}
	s->data[len] = '\0'; //memory returned by malloc isn't initialized to 0. Setting the first byte of new string to zero ensures that it looks like an empty string
	s->length = len;
	return s;
}

//a differenza di strncpy, lo user non si deve preoccupare delle dimensioni della stringa dst
void append_string(string_t *s, const char *toAdd){ //https://stackoverflow.com/questions/5901181/c-string-append
	char *new = malloc(strlen(s->data) + strlen(toAdd) + 1); //strlen(s->data) anzichè s->length poichè l'utente può aver modificato s->data nel mentre
	new[0] = '\0';
	strcat(new, s->data);
	strcat(new, toAdd);
	size_t newLen = strlen(new);
	//liberiamo la precedente
	free(s->data);
	s->data = (char *)malloc(newLen + 1);
	memcpy(s->data, new, newLen);
	free(new);
	s->data[newLen] = '\0';
	s->length = newLen;
}

char *extract_substring_at(string_t *s, int startPos, int endPos){}

//inserts substring at position i into string
void insert_substring_at(string_t *s, const char *toInsert, int startPos){}
//removes substring from string
void remove_substring(string_t *s, const char *toRemove){}

//returns occurrences number of a single char in a string
int substring_occurrences(char *str, const char * sub){ //l'algoritmo ha costo O(n^2)(sommatoria gaussiana) nel caso pessimo
														//non eseguo direttamente questo algoritmo per splittare la stringa perchè non so quanti token dovrò inserire nell'array (qui potrei estrarre ogni substringa, ma non so quanto allocare!)
														//char *str = s->data; //temp per comodità di lettura

	int i = 0, j = 0, k = 0, flag = 0, count = 0;
	for(i = 0 ; *(str + i) != '\0' ; i++){

		k = i; //k è una copia di i in caso ci "illudiamo" di aver trovato una sottostringa (e quindi incrementiamo i) e.g. main=aabc, sub=abc
		while((*(str + i) == *(sub + j)) && flag == 0){
			i++; j++; //se i caratteri coincidono i contatori avanzano assieme 

			if (strlen(sub) == j){
				count++; //un'occorrenza trovata
				flag = 1; //usciamo dal while
			}
		}

		j = 0;//ripristino contatore sottostringa
		if (flag == 0) i = k; //sottostringa non trovata
		else flag = 0; //cerca nuova sottostringa 
	}
	return count;
}
//fills given array with string tokens. Alla fine viene aggiunto un ulteriore elemento: NULL
void split_string(char *str, const char *delim, char** array){

	//e.g. 192.168.0.1 => 3 punti => 3+1 token. array 0 based
	/*Example
#include "davolib.h"
#include <stdio.h>
void main(){
string_t *str = create_string("192.168.0.1");
int len = substring_occurrences(str->data, ".") +1;
char *token[len + 1];//+1 perchè NULL TERMINATED
split_string(str->data, ".", token);
for (int i = 0; i < len; i++){
printf("%s\n", *(token+i));
}
}
*/

//SBAGLIATO!: gli passi il puntatore, non copi la memoria!!! v
//char *str = s->data; //temp per comodità di lettura (strtok cambia offset di lettura?)

//per non ricordarsi ogni volta di passare alla funzione anche la lunghezza dell'array da riempire
	int arrayLen = substring_occurrences(str, delim) + 1;

	char *firstToken = strtok(str, delim); //"idiosincrasia" di strtok
	*(array + 0) = firstToken;


	for(int i = 1 ; i < arrayLen; i++){
		*(array + i) = strtok(NULL, delim); //strtok può avere più delim!
	}

	*(array + arrayLen) = NULL;

}

//get number of elements in dirPath tree(no . and ..)
int getNumDirElems(const char *dirPath){
	int count = 0; //numero di elementi
	DIR *dir = opendir(dirPath);
	//check errors
	if (dir == NULL){
		perror("opendir");
		exit(EXIT_FAILURE);
	}

	string_t *path = create_string(dirPath);
	struct dirent *entity;

	while ((entity = readdir(dir)) != NULL) {

		if(strcmp(entity->d_name, ".") == 0 || strcmp(entity->d_name, "..") == 0) continue;

		//check if file is a directory 
		if (entity->d_type == DT_DIR){ //https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html

			append_string(path, "/");
			append_string(path, entity->d_name);
			count = count + getNumDirElems(path->data);
			update_string(path, dirPath);
		}
		count++; //cartella comunque contata
	}

	free(path);
	closedir(dir);
	return count;
}

//list recursively all relative paths in rootDir tree into given array 
int lsFilesDirTree(const char *rootDir, char **buf, int i){ 
	DIR* dir = opendir(rootDir);
	if (dir == NULL) {
		perror("opendir");
		exit(EXIT_FAILURE);
	}


	string_t *path = create_string(rootDir);
	int j = i; //indice per inserire elementi nell'array dal quale partire.

	struct dirent* entity;
	while ((entity = readdir(dir)) != NULL) {

		if(strcmp(entity->d_name, ".") == 0 || strcmp(entity->d_name, "..") == 0) continue;

		append_string(path, "/");
		append_string(path, entity->d_name);

		if (entity->d_type == DT_DIR) {
			j += lsFilesDirTree(path->data, buf, j);
		}
		
		//inserisco prima i sotto elementi, poi la radice

		*(buf + j) = (char *)malloc(path->length + 1); //Because of NULL terminator
		init_array(*(buf + j));
		strncpy(*(buf + j), path->data, path->length);	

		j++; //anche cartelle contate come elementi dell'albero e da analizzare
		update_string(path, rootDir);
	}
	
	free(path);
	closedir(dir);
	return j-i; //numero di nuovi elementi nella subdir
}
