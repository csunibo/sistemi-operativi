/*
Scrivere il comando absls che mostri per ogni file della directory passata come parametro il path
completo di ogni file (mostrando al posto dei link simbolici il path completo dei file puntati).
*/
//stat call will follow the symbolic link to the target file and retrieve the information for that. Try using lstat instead, it will give you the information for the link
//https://stackoverflow.com/questions/3984948/how-to-figure-out-if-a-file-is-a-link
//https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h> //directory entities
#include <string.h>
#include <limits.h> /* PATH_MAX */
#include "davolib.h"
#include <string.h>
/*
 * This program displays the names of all files in the current directory.
 */
void main(int argc, char const *argv[])
{
	string_t *path = create_string("");
	DIR *dir;

	if (argc < 2){
		dir = opendir(".");
		char cwd[256];
   		if (getcwd(cwd, sizeof(cwd)) != NULL){
   			append_string(path, cwd);
   		}
   		else{
   			perror("getcwd");
   			exit(EXIT_FAILURE);
   		}
	}
	else{
		dir = opendir(argv[1]);
		char tmp[256];
		realpath(argv[1], tmp);
		append_string(path, tmp);
	}

	printf("working in %s\n", path->data);
	//char *copy = path->data; se poi faccio update_string(path, copy); mi dà un risultato strano
	// => https://stackoverflow.com/questions/14992772/modifying-string-literal-passed-in-as-a-function
	char copy[256];
	memcpy(copy, path->data, 256);

	if (dir){

		struct dirent *ent;
		char resolvedPath[256];
		init_array(resolvedPath);

		while ((ent = readdir(dir)) != NULL) {

			/*
			The  readdir()  function returns a pointer to a dirent structure repre-
       		senting the next directory entry in the directory stream pointed to by
       		dir. It returns NULL on reaching the end of the directory stream or
       		if an error occurred.
			*/

			update_string(path, copy); //reset per ogni entità
			init_array(resolvedPath);

			append_string(path, "/");
			append_string(path, ent->d_name);

			if (ent->d_type == DT_LNK){
				realpath(path->data, resolvedPath);
				printf("file %s resolved to %s\n", path->data, resolvedPath);
			}
			else{
				printf("file %s\n", path->data);
			}
    	}

    	closedir(dir);
	}
	else {
		perror("opendir");
		exit (EXIT_FAILURE);
	}
}