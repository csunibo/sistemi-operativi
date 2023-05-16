/**	Esercizio 1
 * Il comando abslink, da implementare, deve sostituire un link simbolico con uno equivalente che sia un riferimento ad un path assoluto.
 * 
 * Ad esempio: se mylink e' nella directory /home/user 
 * e punta a myfile, 'abspath mylink" 
 * deve sostituire mylink con un symbolic link a /home/user/myfile.
 * 
 * Hint: considerate l'uso della funzione realpath
 * 
 * link testo https://csunibo.github.io/sistemi-operativi/prove/pratica/scritto-2020-02-21-testo.pdf
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>// stat
#include <sys/stat.h>//stat
#include <unistd.h>//stat
#include <string.h>// strcat
#include <dirent.h>// dirname

// controllo argomenti
// trovo la cartella di mylink [argv[1]]
// trovo il myile a cui punta
// creo il link assoluto

int main(int argc, char* argv[]){
	if (argc != 2){
		printf("Usage: %s <link>\n", argv[0]);
		return 1;
	}
	
    char* mylink = argv[1];
	struct stat st;
	lstat(mylink, &st);
	if (S_ISLNK(st.st_mode)){
		// trovo la cartella di mylink
		char* dir = dirname(mylink);
		// trovo il myile a cui punta
		char* file = realpath(mylink, NULL);
		// creo il link assoluto
		char* link = strcat(dir, file);
		symlink(link, mylink);
	}
	return 0;
}