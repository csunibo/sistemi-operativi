/* Scrivere un programma ager in grado di invecchiare file. */
/* Il programma deve poter prendere una lista di file come parametri o nessun parametro, nel qual caso invecchierà */
/* tutti I file della directory corrente. “invecchiare” significa predatare il tempo di ultimo accesso e di modifica di 10 */
/* giorni. */
/* Esempio: */
/* $ ls -l file */
/* -rw-r--r-- 1 renzo renzo 0 Feb 17 09:07 file */
/* $ ./ager file */
/* $ ls -l file */
/* -rw-r--r-- 1 renzo renzo 0 Feb 7 09:07 file */
/* Completare l’esercizio 1 ponendo una gestione opportuna dei parametri in linea comando che preveda di */
/* poter modificare solo la data di accesso o quella di modifica, di definire il tempo di “invecchiamento” e un help. */
/* Si possono inserire altri parametri ritenuti opportuni. */
/* https://rosettacode.org/wiki/File_modification_time#C */
/* #include <getopt.h> */
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <utime.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define TRUE 1
#define FALSE 0
#define BUFSIZE 512
#define FILES_TO_CMP 2
#include <ctype.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
int main(int argc, char **argv){
    int opt ;
    if ( argc < 2 ){
        printf("\nUSAGE: %s -m new_mtime -t new_atime", argv[0]);
        exit(EXIT_SUCCESS);
    }
    time_t new_atime = -1, new_mtime = -1;
    int atime = FALSE, mtime = FALSE;


    /* https://cboard.cprogramming.com/linux-programming/72100-getopt-multiple-options.html */
    while((opt = getopt(argc, argv, "a:m:") != -1)){
        switch (opt) {
            /* case 'h': */
            /*     printf("\nUSAGE: %s -m new_mtime -t new_atime", argv[0]); */
            /*     exit(EXIT_SUCCESS); */
            /*     break; */
            case 'a':
                atime = TRUE;
                printf("SONO QUI");
                fprintf(stderr, "\noptarg = %s", optarg);
                new_atime = atoi(optarg);
                break;
            case 'm':
                mtime = TRUE;
                new_mtime = atoi(optarg);
                break;
            case '?':
                fprintf(stderr, "\nNOT RECOGNIZED");
                break;
            /* default: */
            /*     printf("\nSEE USAGE WITH %s -h", argv[0]); */
            /*     exit(EXIT_SUCCESS); */
        }
    }

    /* if (optind >= argc) { */
    /*     fprintf(stderr, "Expected argument after options\n"); */
    /*     exit(EXIT_FAILURE); */
    /* } */

    fprintf(stderr, "\nmtime = %d, atime = %d", mtime, atime);

    DIR *dir = opendir(".");
    if (dir == NULL) HANDLE_ERROR("\nopendir()");

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL){
        fprintf(stderr, "\nsto analizzando %s", ent->d_name);
        if ( (strcmp(ent->d_name, ".") != 0 ) && ( strcmp(ent->d_name, "..") != 0 ) ) {
            struct stat buf; bzero(&buf, sizeof(struct stat));
            if (stat(ent->d_name, &buf) == -1) HANDLE_ERROR("\nstat()");
            /* ctime is *not* the creation time. It's the change time of the inode. */
            /*     It's updated whenever the inode is modified, e.g. metadata modifications */
            /*     like permission changes, link/unlink of hard links etc. */
            time_t mtime = buf.st_mtime; /* seconds since the epoch */
            time_t atime = buf.st_atime;
            fprintf(stderr, "\nI'm parsing %s", ent->d_name);
            struct utimbuf new_times; bzero(&new_times, sizeof(struct utimbuf));
            new_mtime == -1 ? ( new_times.modtime = buf.st_mtime ) : (new_times.modtime = buf.st_mtime - new_mtime*86400);
            new_atime == -1 ? ( new_times.actime = buf.st_atime ) : (new_times.actime = buf.st_atime - new_atime*86400);
            if (utime(realpath( ent->d_name , NULL), &new_times) == -1){
                HANDLE_ERROR("\nutime()");
            }
        }
    }
    exit(EXIT_SUCCESS);
}

/* https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/ */
/* const char *s_option="hi:n:p:v"; */
/*         const struct option l_option[]={ */
/*                  { "help",      0, NULL, 'h'}, */
/*                  { "ip",        1, NULL, 'i'}, */
/*                  { "nm",        1, NULL, 'n'}, */
/*                  { "port",      1, NULL, 'p'}, */
/*                  { "version",   0, NULL, 'v'}, */
/*         }; */

/* // Now, looking for the supplied arguments and options */
/*         do { */
/*                 n_opt=getopt_long(argc,argv,s_option,l_option,NULL); */
/*                 switch(n_opt){ */
/*                         case 'h': */
/*                         case '?': */
/*                                 usage(argv[0],0); */
/*                                 break; */
/*                         case 'i': */
/*                                 printf("i\n%s\n",optarg); */
/*                                 break; */
/*                         case 'n': */
/*                                 printf("n\n%s\n",optarg); */
/*                                 break; */
/*                         case 'p': */
/*                                 printf("p\n%s\n",optarg); */
/*                                 break; */
/*                        case 'v': */
/*                                 printf(" %s version %s\n",argv[0],VERSION_INFO);                                                     exit(0); */
/*                         case -1: */
/*                                 break; */
/*                         default: */
/*                                 abort(); */
/*                 } /1* switch *1/ */
/*         } while (n_opt!=-1); */
