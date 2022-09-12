/* scrivere la funzione: */
/* void printgroupdir(const char *dirp, const char *group); */
/* che deve scandire la directory con pathname dirp e stampare tutti I file (non le directory) che appartengano al */
/* gruppo passato come secondo parametro. */
/* Creare I file printgroupdir.c, printgroupdir.h e un file testpgd.c contenente un programma principale che prenda */
/* come argv[1] e argv[2] i due parametri della funzione. Produrre un Makefile opportuno. */
/* Esempi per provare il funzionamento possono essere: */
/* testpgd /dev tty */
/* testpgd /dev disk */
#include "printgroupdir.h"
#include <stdio.h>
/* https://stackoverflow.com/questions/21023605/initialize-array-of-strings */
char **groupdir(const char *dirp, const char *group){
    char **res = (char **)malloc(MAX_FILES*sizeof(char *));
    for (int j=0; j< MAX_FILES; j++){
        res[j] = (char *)malloc(PATH_MAX*sizeof(char));
        bzero(res[j], PATH_MAX);
    }

    int i = 0;
    DIR *dir = opendir(dirp);
    if (dir == NULL) HANDLE_ERROR("\nopendir()");
    struct dirent *ent; //bzero(ent, sizeof(struct dirent));
    while ((ent = readdir(dir)) != NULL){
        if ( ent->d_type != DT_DIR){
            struct stat buf; bzero(&buf, sizeof(struct stat));
            char path[PATH_MAX]; bzero(path, sizeof(path));
            snprintf(path, PATH_MAX, "%s/%s", dirp, ent->d_name);
            if (stat(path, &buf) == -1) HANDLE_ERROR("\nstat()");
            if (strcmp( getgrgid(buf.st_gid)->gr_name, group) == 0 ){
                snprintf(res[i], PATH_MAX, "%s", path);
                i++;
            }
        }
    }
    res[i] = (char *)0;
    /* snprintf(res[i], PATH_MAX, "%s", (char *)0); */ //commentato altrimenti usciva (null) formattato
    return res;
}
void freestringarray(char **v){
    /* https://stackoverflow.com/questions/4733649/freeing-memory-which-has-been-allocated-to-an-array-of-char-pointers-strings */
    fprintf(stderr, "\nDENTRO SEC FUNC");
    for (int i=0; i<MAX_FILES; i++){
        /* bzero(*(v + i), PATH_MAX); */
        free(v[i]);
        v[i] = NULL;
    }
    free(v);
    v = NULL;
}
