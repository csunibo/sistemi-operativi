/**	Esercizio 2
 * Scrivere il comando absls che mostri per ogni file della directory passata come parametro 
 * il path completo di ogni file 
 * (mostrando al posto dei link simbolici il path completo dei file puntati).
 * 
 * link testo https://csunibo.github.io/sistemi-operativi/prove/pratica/scritto-2020-02-21-testo.pdf
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> // struct dirent *readdir(DIR *directory_pointer);

// controllo argomenti
// prendo il path della directory dai parametri
// leggo la directory
// stampo il path absolute
// se il file e' un link simbolico, stampo il path assoluto del file a cui punta

int main(int argc, char* argv[]){
	if (argc != 2){
		printf("Usage: %s <directory>\n", argv[0]);
		return 1;
	}
	
	char* dir = argv[1];
	struct dirent *entry;
	DIR *dp = opendir(dir);
	return 0;
}