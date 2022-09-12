#include <linux/limits.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <strings.h>
#include <unistd.h>
#include <limits.h>
#include <grp.h>
#define MAX_FILES 256
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define TRUE 1
#define FALSE 0
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#define HANDLE_ERROR(msg) \
    do { perror(msg); return NULL; } while (0)
char **groupdir(const char *dirp, const char *group);
void freestringarray(char **v);
