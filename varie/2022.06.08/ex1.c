/* Scrivere il programma invsymlink che prende come parametro il pathname di un file. */
/* Se il parametro è un link simbolico il programma inverte il link simbolico: il file puntato viene messo al */
/* posto del file link simbolico e nella precedente collocazione dove era il file puntato dal link simbolico */
/* viene messo un link simbolico che punti alla nuova collocazione del file. (*) */
/* Se A e B indicano lo stesso file perché B è un link simbolico che punta ad A, allora "invsymlink B" deve */
/* fare in modo che B diventi il file che precedentemente era A e A diventi un link simbolico che punti a */
/* B. */
/* (*) usare il pathname completo che può essere calcolato con realpath(3) */
/* Estendere il programma invsymlink: se il parametro è una directory e non un file allora tutti i link */
/* simbolici presenti nella directory devono venir "invertititi". */
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <libgen.h>
#include <sys/sendfile.h>
#include <dirent.h>

void swap(char *argv);
int main (int argc, char **argv){
    struct stat buf;
    memset(&buf, 0, sizeof(struct stat));

    if (argc < 2) {
        printf("\nBAD USAGE");
        exit(EXIT_FAILURE);
    }
    /* https://stackoverflow.com/questions/3984948/how-to-figure-out-if-a-file-is-a-link */
    /* lstat(argv[1], &buf); */
    /* if (!S_ISLNK(buf.st_mode)) { */
    /*     printf("\n%s not a soft link", argv[1]); */
    /*     exit(EXIT_FAILURE); */
    /* } */
    stat(argv[1], &buf);
    if (!S_ISDIR(buf.st_mode)) {
        printf("\n%s isn't a directory", argv[1]);
        exit(EXIT_FAILURE);
    }

    char respath[PATH_MAX]; memset(respath, 0,sizeof(respath));
    realpath(argv[1], respath);
    char cpy[PATH_MAX]; memset(cpy, 0, sizeof(cpy));

    DIR *dir = opendir(argv[1]);
    if (dir == NULL){
        perror("\nopendir");
        exit(EXIT_FAILURE);
    }
    struct dirent *ent;
    memset(ent, 0, sizeof(struct dirent));

    while ((ent = readdir(dir)) != NULL){
        if (ent->d_type == DT_LNK){
            //reset
            memset(cpy, 0, sizeof(cpy));
            strncat(cpy, respath, PATH_MAX);
            strncat(cpy, "/", PATH_MAX);
            strncat(cpy, ent->d_name, PATH_MAX);
            swap(cpy);
        }
    }

}

void swap(char *argv){

    char respath[PATH_MAX], resbasename[PATH_MAX], resdirname[PATH_MAX], linkbasename[PATH_MAX], linkdirname[PATH_MAX];
    memset(respath, 0, sizeof(respath));
    memset(resbasename, 0, sizeof(resbasename));
    memset(resdirname, 0, sizeof(resbasename));

    memset(linkbasename, 0, sizeof(resbasename));
    memset(linkdirname, 0, sizeof(resbasename));
    struct stat buf;
    lstat(argv, &buf);
    printf("\nargv: %s", argv);
    char *resolved = realpath(argv, NULL);
    if (resolved == NULL){
        perror("\nrealpath");
        exit(EXIT_FAILURE);
    }
    //ABS PATH
    snprintf(resbasename, sizeof(resbasename), "%s", basename(resolved));
    snprintf(resdirname, sizeof(resbasename), "%s/", dirname(resolved));

    snprintf(linkbasename, sizeof(resbasename), "%s", basename(argv));
    snprintf(linkdirname, sizeof(resbasename), "%s/", dirname(argv));

    printf("\nresbasename: %s, resdirname: %s, linkbasename: %s, linkdirname: %s", resbasename, resdirname, linkbasename, linkdirname);

    int fd_link_dir = open(linkdirname, O_RDONLY);
    if (fd_link_dir < 0){
        perror("\nfd_link_dir");
        exit(EXIT_FAILURE);
    }
    //creo/sarà il file copiato
    int fd_in_link_dir = openat(fd_link_dir, resbasename, O_CREAT | O_RDWR, 0777);
    if (fd_in_link_dir < 0){
        perror("\nfd_in_link_dir");
        exit(EXIT_FAILURE);
    }

    int fd_res_dir = open(resdirname, O_RDONLY);
    if (fd_res_dir < 0){
        perror("\nfd_res_dir");
        exit(EXIT_FAILURE);
    }
    int old_fd_res_in_dir = openat(fd_res_dir, resbasename, O_RDWR);
    if (old_fd_res_in_dir < 0){
        perror("\nold_fd_res_in_dir");
        exit(EXIT_FAILURE);
    }
    //...sarà il nuovo link...
    printf("\nresolved: %s", resolved);
    //TODO se -1 e errno != OVERFLOW
    sendfile(fd_in_link_dir, old_fd_res_in_dir, 0, buf.st_size);
    free(resolved);
    printf("\nHo copiato il file(link risolto) nella cartella del link");

    char new_link[PATH_MAX], new_res[PATH_MAX];
    memset(new_link, 0, sizeof(new_link));
    memset(new_res, 0, sizeof(new_link));

    strncat(new_res, linkdirname, PATH_MAX);
    strncat(new_res, resbasename, PATH_MAX);

    strncat(new_link, resdirname, PATH_MAX);
    strncat(new_link, linkbasename, PATH_MAX);

    printf("\nnew_res: %s, new_link: %s", new_res, new_link);


    //link relative to fd_link_dir
    if (symlink(realpath(new_res, NULL),  new_link) < 0){
        perror("\nsymlinkat");
        exit(EXIT_FAILURE);
    }
    unlink(argv);

}
