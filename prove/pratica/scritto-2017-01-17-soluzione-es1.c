#include <dirent.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define MAX 51200
ino_t inode_array[MAX];
size_t pos = 0;

size_t files = 0, dirs = 0;

bool find(ino_t exp) {
  for (size_t i = 0; i <= pos; ++i)
    if (inode_array[i] == exp)
      return true;

  return false;
}

void count(DIR *d, char *str) {
  char path[PATH_MAX];
  struct dirent *e;
  while ((e = readdir(d)) != NULL) {
    if (!(e->d_type == DT_DIR || e->d_type == DT_REG))
      continue;
    if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
      continue;

    if (!find(e->d_ino)) {
      inode_array[pos++] = e->d_ino;
      if (e->d_type == DT_DIR) {
        ++dirs;
        strcpy(path, str);
        strcat(path, "/");
        strcat(path, e->d_name);
        DIR *d = opendir(path);
        if (d == NULL) {
          perror("Could not open subdir");
          exit(1);
        }
        count(d, path);
      } else
        ++files;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc > 3 || argc < 1) {
    printf("Usage: %s [pram]", argv[0]);
    exit(1);
  }
  char *dir;
  if (argc == 1)
    dir = ".";
  else
    dir = argv[1];

  DIR *d = opendir(dir);
  if (d == NULL) {
    perror("Could not open dir");
    exit(1);
  }

  for (size_t i = 0; i <= MAX; ++i)
    inode_array[i] = 0;

  count(d, dir);

  printf("dir: %d\n", dirs);
  printf("files: %d\n", files);

  exit(0);
}
