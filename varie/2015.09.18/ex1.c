/* La directory /proc contiene un file system virtuale utile per consultare o modificare strutture dati del kernel. */
/* All'interno di tale directory c'e' una sottodirectory per ogni processo attivo nel sistema (riconoscibile perche' il nome */
/* della sottodirectory e' il numero del processo). */
/* Scrivere un programma che stampi il numero dei processi attivi e rimanga in attesa controllando ogni secondo */
/* quanti siano. In caso di variazione del numero dei processi stampa il nuovo numero. */
/* Es: */
/* $ countproc */
/* 230 */
/* 232 */
/* 230 */
/* countprocuser e' una estensione del programma dell'esercizio 1. */
/* Se viene indicato come parametro il nome di un utente deve contare non gia' tutti I processi ma solo quelli */
/* dell'utente specificato. */
/* https://www.linuxjournal.com/article/8478?page=0,0 */
/* https://stackoverflow.com/questions/24897799/why-cant-i-monitor-proc-using-the-inotify-c-api-is-it-special */
         //https://unix.stackexchange.com/questions/90627/notify-of-changes-on-a-file-under-proc
/* https://stackoverflow.com/questions/8268316/how-to-retrieve-the-user-name-from-the-user-id */
#include <linux/limits.h>
#include <stdlib.h>
#include <dirent.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define TRUE 1
#define FALSE 0
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include <pwd.h>
int digits_only(const char *s)
{
    while (*s) {
        if (isdigit(*s++) == 0) return FALSE;
    }
    return TRUE;
}
int main(int argc, char **argv){
    char directory[PATH_MAX]; INIT_ARRAY(directory);
    snprintf(directory, PATH_MAX, "%s", "/proc");
    int pre_running_proc = 0;
    while (1){
        DIR *dd = opendir(directory); //mettendolo dentro al while, non stampa più 0...
        if (dd == NULL) HANDLE_ERROR("opendir()1\n");
        sleep(1);
        int running_procs = 0;
        struct dirent *ent; //NON ZEROING!
        while( (ent = readdir(dd)) != NULL ){
            if ( strcmp(ent->d_name, ".") == 0 ) continue;
            if ( strcmp(ent->d_name, "..") == 0 ) continue;
            if (ent->d_type == DT_DIR){
                if (digits_only(ent->d_name)){
                    if (argc == 2){
                        struct stat buf; bzero(&buf, sizeof(struct stat));
                        char path[PATH_MAX]; INIT_ARRAY(path);
                        snprintf(path, PATH_MAX, "%s/%s", directory, ent->d_name);
                        if (stat(path, &buf) == -1) HANDLE_ERROR("stat()1\n");
                        if ( strcmp(getpwuid(buf.st_uid)->pw_name, argv[1]) == 0 ) running_procs ++;
                    }
                    else running_procs++;
                }
            }
        }
        if (running_procs != pre_running_proc) printf("running_procs: %d\n", running_procs);
        pre_running_proc = running_procs;
        closedir(dd);
    }

    exit(EXIT_SUCCESS);
}
