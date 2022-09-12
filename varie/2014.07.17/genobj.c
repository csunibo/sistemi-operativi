/* Scrivere un programma che per ogni file .c nella directory corrente chiami il compilatore gcc per generare il file oggetto (.o) a */
/* meno che esista gia' nella directory un file oggetto relativo allo stesso sorgente che sia piu' nuovo (ultima modifica) del */
/* sorgente. */
/* Tutti I parametri devono essere passati al compilatore. */
/* Es: */
/* genobj -I . -ggdb */
/* se nella directory corrente esistono I file uno.c e due.c e il file due.o deve richiamare */
/* gcc -I. -ggdb -c uno.c */
/* e, solo se due.o ha ultima modifica precedente a due.c, deve chiamare */
/* gcc -I. -ggdb -c due.c */


#include <linux/limits.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#define MAX_C_FILES 128
#define MAX_O_FILES MAX_C_FILES
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define HANDLE_ERROR(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)
void compile(char **argv){
	pid_t pid = fork();
	if (pid == -1) HANDLE_ERROR("fork");
	else if (pid == 0){
		execvp(argv[0], argv);
		HANDLE_ERROR("execvp");
	} else{
		/* int status; wait(&status); */
	}
}
long get_edit_time(char *path){
	struct stat buf; bzero(&buf, sizeof(buf));
	if (stat(path, &buf) == -1) HANDLE_ERROR("stat");
	return buf.st_mtime;
}

char *get_filename_name(char *path){
	/* https://stackoverflow.com/questions/5309471/getting-file-extension-in-c */
	const char *dot = strrchr(path, '.');
	if(!dot || dot == path) return path;
	char *buf = malloc(PATH_MAX*sizeof(char));
	snprintf(buf, dot - path +1, "%s", path);
	return buf;
}

const char *get_filename_ext(char *filename){
	/* https://stackoverflow.com/questions/5309471/getting-file-extension-in-c */
	const char *dot = strrchr(filename, '.');
	if(!dot || dot == filename) return "";
	return dot + 1;
}
void find_files_by_ext(char *dirname, char **files, char *ext){
	DIR *dir = opendir(dirname);
	if (dir == NULL) HANDLE_ERROR("opendir");
	struct dirent *ent;
	int i = 0;
	while((ent = readdir(dir)) != NULL){
		if (ent->d_type == DT_REG){
			if (strcmp(ext, get_filename_ext(ent->d_name)) == 0){
				fprintf(stderr, "found %s file\n", ext);
				snprintf(files[i], PATH_MAX, "%s", ent->d_name);
				i++;
			}
		}
	}
	closedir(dir);
}
int main(int argc, char **argv){
	char **c_files = (char **)malloc(MAX_C_FILES*sizeof(char *));
	char **o_files = (char **)malloc(MAX_O_FILES*sizeof(char *));
	char **new_argv = (char **)malloc((argc-1+2 +1+2)*sizeof(char *)); //togliamo il nome dell'eseguibile, aggiungiamo gcc, nome del file in c, -o e nome,  NULL

	for (int i=0; i<MAX_C_FILES; i++){
		if (i<(argc-1+2+1+2)) new_argv[i] = (char *)calloc(PATH_MAX, sizeof(char));
		c_files[i] = (char *)calloc(PATH_MAX, sizeof(char));
		o_files[i] = (char *)calloc(PATH_MAX, sizeof(char));
	}

	snprintf(new_argv[0], PATH_MAX, "%s", "gcc");
	snprintf(new_argv[argc+1], PATH_MAX, "%s", "-o");
	new_argv[argc+1+2] = NULL;
	for (int i=1; i< argc; i++) snprintf(new_argv[i], PATH_MAX, "%s", argv[i]);


	find_files_by_ext(".", c_files, "c");
	find_files_by_ext(".", o_files, "o");
	//per ogni file, controllo se c'è un file object che ha il mio stesso nome. Dopodichè faccio le verifiche

	for (int i=0; i<MAX_C_FILES; i++){
		if (strlen(c_files[i]) == 0) break;
		int found_obj = 0;
		fprintf(stderr, "analizing %s\n", c_files[i]);
		for (int j=0; j<MAX_O_FILES; j++) {
			char *name_c = get_filename_name(c_files[i]);
			char *name_o = get_filename_name(o_files[j]);
			if (strcmp(name_c, name_o) == 0){
				fprintf(stderr, "stesso match: %s\n", name_c);
				found_obj = 1;
				if (get_edit_time(c_files[i]) > get_edit_time(o_files[j])){
					fprintf(stderr, "%s.c is newer\n", c_files[i]);
					snprintf(new_argv[argc], PATH_MAX, "%s", c_files[i]);
					snprintf(new_argv[argc+2], PATH_MAX, "%s", name_c);
					compile(new_argv);
					snprintf(new_argv[argc], PATH_MAX, "%s", "");
					snprintf(new_argv[argc+2], PATH_MAX, "%s", "");
				}
				else {fprintf(stderr, "%s.c is older...\n", c_files[i]);}
			}
			/* free(name_o); free(name_c); */
			if (found_obj) break;
		}
		if (found_obj != 1){
			snprintf(new_argv[argc], PATH_MAX, "%s", c_files[i]);
			snprintf(new_argv[argc+2], PATH_MAX, "%s", get_filename_name(c_files[i]));
			compile(new_argv);
			snprintf(new_argv[argc], PATH_MAX, "%s", "");
			snprintf(new_argv[argc+2], PATH_MAX, "%s", "");
		}
	}
	free(c_files); free(new_argv); free(o_files);
	exit(EXIT_SUCCESS);
}
