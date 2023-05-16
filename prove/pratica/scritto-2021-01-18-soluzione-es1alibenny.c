/** Esercizio 1
 * Scrivere il programma abyssmlink che ha come parametro il pathname di una directory.
 * Il programma deve cercare tutti i link simbolici presenti nella directory 
 * e trasformare ogni link simbolico in uno equivalente al pathname assoluto e non relativo.
 *  
 * link testo: https://csunibo.github.io/sistemi-operativi/prove/pratica/scritto-2021-01-18-testo.pdf
*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>	// lstat
#include <sys/stat.h>	// lstat
#include <unistd.h>	// lstat
#include <dirent.h>	// opendir
#include <fcntl.h>	// open
#include <limits.h>	// PATH_MAX

int main (int argc, char* argv){
	if(argc!= 2){
        printf("Usage: %s <directory>\n", argv[0]);
        return -1;
    }
	char pathname = argv[1];
    struct stat st;
	lstat(pathname, &st);
    if(S_ISDIR(st.st_mode)){	// e` una directory?
		DIR *dir = opendir(pathname);
		struct dirent *ent;
		while (ent = readdir(dir) != NULL){		// fino a quando riesce a leggere la directory
			int file_desc = open(pathname, O_RDONLY);	// apro il file in read-only
			if (file_desc != -1){	// non c'e` errore?
				struct stat filestat;
				lstat(entry->d_name, &filestat);
				if (S_ISLNK(filestat.st_mode)){		// il file e` davvero un link
					//sostituisco il file con la copia
					char real_path[1024];	// 1024 caratteri max
					realpath(entry->d_name, real_path);
					close(file_desc);
					char real_name[1024];
					strcpy(real_name, entry->d_name);	// salvo il nome del file
					unlink(entry->d_name);
					copyfile(real_path, real_name);                
				}
				else		// il file non e` un link
					close(file_desc);
			}
		}
	}else
		printf("Il file non e` una directory\n");
}