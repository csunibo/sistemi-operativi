#include <stdio.h>
#include <linux/limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <limits.h> /* PATH_MAX */

typedef struct string {
	size_t length;
	char *data;
} string_t;

void cerror(int exp, const char *msg);

#define init_array(T) ( memset((T), 0, sizeof((T))) )

void _pause();

string_t* create_string(const char *str);
void append_string(string_t *s, const char *toAdd);
int substring_occurrences(char *str, const char * sub);
void split_string(char *str, const char *delim, char** array);
void update_string(string_t* s, const char *newStr);

int appendFileToFile(const char *toAppendPath, const char *dstPath);
int getFileSize(const char *filePath);
int getOutput(const char *filePath, char const *argv[], char *buf, int bufSize);


int readWholeFile(const char *filePath, char *buf);
int getNumDirElems(const char *dirPath);
int lsFilesDirTree(const char *rootDir, char **buf, int i);
