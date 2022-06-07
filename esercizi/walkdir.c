#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_PATH 1024

void dirwalk(const char *dir, void (*fcn)(char *), bool recursive);
void f(char *);

int main(int argc, char *argv[]) { dirwalk(".", f, true); }

void dirwalk(const char *dir, void (*fcn)(char *), bool recursive) {
  char path[MAX_PATH];
  struct dirent *dp;
  DIR *dfd;

  if ((dfd = opendir(dir)) == NULL) {
    fprintf(stderr, "dirwalk: non riesco ad aprire %s\n", dir);
    return;
  }
  while ((dp = readdir(dfd)) != NULL) {
    if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
      continue;

    if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(path))
      fprintf(stderr, "dirwalk: il nome %s/%s e' troppo lungo\n", dir,
              dp->d_name);
    else {
      sprintf(path, "%s/%s", dir, dp->d_name);
      if (dp->d_type == DT_DIR && recursive)
        dirwalk(path, fcn, true);
      else if (dp->d_type == DT_REG)
        (*fcn)(path);
    }
  }
  closedir(dfd);
}

void f(char *path) { printf("%s\n", path); }
