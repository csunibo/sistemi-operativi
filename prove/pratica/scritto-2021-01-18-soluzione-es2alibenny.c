/** Esercizio 2
 * Scrivere cpsymlink: che ha come parametro il pathname di una directory.
 * Il programma deve cercare tutti i link simbolici presenti nella directory 
 * e sostituire ogni link simbolico che punta ad un file regolare con la copia 
 * del file puntato.
 * 
 * link testo: https://csunibo.github.io/sistemi-operativi/prove/pratica/scritto-2021-01-18-testo.pdf
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>	// stat, lstat
#include <sys/stat.h>	// stat, lstat
#include <unistd.h>		// stat, lstat
#include <fcntl.h>		// stat, lstat
#include <sys/sendfile.h>	// sendfile
#include <dirent.h>		// opendir, readdir

// copiare un file da una directory all'altra
int copyfile(const char *directory, const char *name){
	int fd = open(directory, O_RDONLY);
	if (fd == -1) {
		perror("open");
        return -1;    
	}
	int fd_copy = open(name, O_WRONLY | O_CREAT, 0666);		// 0666 = mode_t mode = e` il set of permission
	struct stat stat_buf;
	lstat(directory, &stat_buf);
	sendfile(fd_copy, fd, 0, stat_buf.st_size);
	close(fd_copy);
	close(fd);
    return 0;
}

int main(int argc, char* argv){
	if (argc != 2){
		printf("Numero parametri errato!");
		exit(EXIT_FAILURE);
	}
	char pathname = argv[1];
	struct stat s;	// statistiche di symlink

	lstat(pathname, &s);		// controllo se il pathname ha sym link
	if (S_ISDIR(s.st_mode)){
		DIR *directory = opendir(pathname);
		struct dirent *entry;	// directory entry
		while ((entry = readdir(directory))!= NULL){
			int fd = open(entry->d_name, O_RDONLY);	// file descriptor
			if (fd != -1){
				struct stat filestat;
				lstat(entry->d_name, &filestat);
				if (S_ISLNK(filestat.st_mode)){		// il file e` davvero un link
					//sostituisco il file con la copia
					char real_path[1024];	// 1024 caratteri max
					realpath(entry->d_name, real_path);
					close(fd);
					char real_name[1024];
					strcpy(real_name, entry->d_name);	// salvo il nome del file
					unlink(entry->d_name);
					copyfile(real_path, real_name);                
				}
				else		// il file non e` un link
					close(fd);
			}
		}
	}
	else 
	    exit(EXIT_FAILURE);

	return 0;
}