
/*
Scrivere un programma searchlink che dati due parametri (nell'ordine un file f ed una directory d)
metta in output l'elenco dei path all'interno dell'albero che ha radice in d che fanno riferimento ad f
o come link fisico o come link simbolico.

es:

searchlink myfile mydir
link a
link d/b
symlink e/s

significa che dir/a e dir/d/b si riferisono a f come link fisici mentre dir/e/s e' un link simbolico che punta ad f.
*/
/* Estendere il programma precedente in modo che accetti le seguenti opzioni: */
/* -c copia il file originale al posto di tutti i link */
/* -l trasforma tutti i link in link fisici */
/* -s trasforma tutti i link in link simbolici */

#include "davolib.h"
#include <fcntl.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void rmf(const char *filePath){
	unlink(filePath);
	remove(filePath);
}

//to get file permissions number:
//https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c
//https://c-for-dummies.com/blog/?p=4101
int getFilePermissions(const char *filePath){
	int number = 0;
	struct stat fileStat;
	stat(filePath, &fileStat);

	/* S_ISDIR(fileStat.st_mode) ? "d" : "-"; */
    if (fileStat.st_mode & S_IRUSR) number+=400;
    if (fileStat.st_mode & S_IWUSR) number+=200;
    if (fileStat.st_mode & S_IXUSR) number+=100;
    if (fileStat.st_mode & S_IRGRP) number+=40 ;
    if (fileStat.st_mode & S_IWGRP) number+=20 ;
    if (fileStat.st_mode & S_IXGRP) number+=10 ;
    if (fileStat.st_mode & S_IROTH) number+=4  ;
    if (fileStat.st_mode & S_IWOTH) number+=2  ;
    if (fileStat.st_mode & S_IXOTH) number+=1  ;
	
 printf("%d", number);
	return  number;
}                                               

int main(int argc, char const *argv[]){

	int len = getNumDirElems(argv[3]); //numero di file nella directory 
	char *list[len]; init_array(list);
	lsFilesDirTree(argv[3], list, 0); //popola array con path relative di tutti elementi del dirtree
	//gli hard link, a differenza dei soft link, hanno lo stesso inode del file al quale puntano
	
	/* -c copia il file originale al posto di tutti i link */
	//strcmp restituisce 0 se le stringhe sono identiche => invertito
	int linkToFile = strcmp(argv[1], "-c") ? 0 : 1;

	/* -l trasforma tutti i link in link fisici */
	int linkToHard =  strcmp(argv[1], "-l") ? 0 : 1;

	/* -s trasforma tutti i link in link simbolici */
	int linkToSoft =  strcmp(argv[1], "-s") ? 0 : 1;

	struct stat buf = {0};
	char resolvedFile[PATH_MAX]; init_array(resolvedFile);
	realpath(argv[2], resolvedFile);
	lstat(resolvedFile, &buf);
	ino_t inode = buf.st_ino;
	/* https://stackoverflow.com/questions/16680302/c-copy-file-permissions-from-another-file */
	/* mode_t permissionsNumber = (mode_t) getFilePermissions(resolvedFile); */
	mode_t permissionsNumber = buf.st_mode;

	char resolvedPath[PATH_MAX]; //verrà usato per ogni file

	//creo cache per "-c"
	char contenuto[getFileSize(resolvedFile) +1];
    init_array(contenuto);
    readWholeFile(resolvedFile, contenuto);
    //https://stackoverflow.com/questions/14992772/modifying-string-literal-passed-in-as-a-function
    /* printf("\nsono pesante %d (compreso \'\\0\') e contengo:\n%s\n mia strlen (sarà pesante-1): %d\n", lunghezza, contenutoStringato, strlen(contenutoStringato)); */

	for(int i=0; i < len; i++){
		
		//reset strutture (memset 0ed)
		init_array(&buf);
		init_array(resolvedPath);
	
		//ln origin link => hard link creato; se -s => soft link
		/* https://stackoverflow.com/questions/3984948/how-to-figure-out-if-a-file-is-a-link */
		/* https://stackoverflow.com/questions/40564474/how-to-use-lstat-to-determine-if-hard-link-or-not#:~:text=The%20only%20way%20to%20reliably,point%20to%20the%20same%20value.&text=is%20not%20useful-,Show%20activity%20on%20this%20post.,links%20to%20actual%20file%20content. */
		
		//per determinare se èè un file o un link
		lstat(*(list + i), &buf);
		realpath(*(list + i), resolvedPath);
		//file stesso
		if (strcmp(resolvedPath, resolvedFile) == 0 && S_ISREG(buf.st_mode)) continue;

		//hard link
		if (S_ISREG(buf.st_mode) && buf.st_ino == inode){
			printf("\n%s link", *(list + i));
			if (linkToFile){
				
				//remove link
				rmf(*(list + i));
				//create copy
				int f = open(*(list + i), O_WRONLY|O_CREAT);
				chmod(*(list + i), permissionsNumber);
				write(f, contenuto, strlen(contenuto));
				close(f);

			}
			else if (linkToSoft){
				rmf(*(list + i));
				symlink(resolvedPath, *(list + i)); //crea link simbolico
			}
			continue;
		}

		//soft link - resolvedFile porterebbe al file!
		/* realpath(*(list + i), resolvedPath); */
		if(S_ISLNK(buf.st_mode) && strcmp(resolvedPath, resolvedFile) == 0){
			
			printf("\n%s symlink", *(list + i));

			if (linkToFile){
	
				//remove link
				rmf(*(list + i));
				//create copy
				int f = open(*(list + i), O_WRONLY|O_CREAT);
				chmod(*(list + i), permissionsNumber);
				write(f, contenuto, strlen(contenuto));
				close(f);

			}
			else if (linkToHard){
				rmf(*(list + i));
				
				link(resolvedPath, *(list + i)); //crea link simbolico
			}
		}
	}
	return 0;
}
