#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  int path_max;
  char *src, *real, *copy;
  DIR *d;
  struct dirent *de;
  struct stat s;

#ifdef PATH_MAX
  path_max = PATH_MAX;
#else
  path_max = pathconf(path, _PC_PATH_MAX);
  if (path_max <= 0)
    path_max = 4096;
#endif

  if (argc != 2) {
    printf("Usage: %s FILE\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  src = argv[1];
  real = (char *)malloc(path_max);
  copy = (char *)malloc(path_max);
  d = opendir(".");
  errno = 0;
  while ((de = readdir(d)) != NULL) {
    if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
      continue;
    strncpy(copy, src, strlen(src));
    if (src[strlen(src) - 1] != '/')
      copy[strlen(src)] = '/';
    strncpy(copy + strlen(src) + 1, de->d_name,
            strlen(de->d_name) + 1); // copy \0
    printf("!! %s\n", copy);
    if (realpath(copy, real) == NULL) {
      perror("realpath");
      exit(EXIT_FAILURE);
    }
    printf("-- %s\n", real);
  }
  errno = 0;
  closedir(d);
  if (errno != 0) {
    perror("readdir");
    exit(EXIT_FAILURE);
  }

  free(real);
  free(copy);
}
