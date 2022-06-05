#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int path_max;
  char *src, *real;

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
  if (realpath(src, real) == NULL) {
    perror("realpath");
    exit(EXIT_FAILURE);
  }

  if (unlink(src)) {
    perror("unlink");
    exit(EXIT_FAILURE);
  }
  if (symlink(real, src)) {
    perror("symlink");
    exit(EXIT_FAILURE);
  }

  free(real);
}
