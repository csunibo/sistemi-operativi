#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define ES2
#define MAX_PATH 1024
#define MAX_CHILDREN 1024

#ifdef ES2
void dirwalk(const char *dir, void (*fcn)(char *), bool recursive);
#endif
void invsymlink(char *);

int main(int argc, char *argv[]) {
  struct stat stbuf;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <path>\n", argv[0]);
    return -1;
  }
  if (lstat(argv[1], &stbuf) == -1) {
    fprintf(stderr, "%s: non riesco ad aprire %s\n", argv[0], argv[1]);
    fprintf(stderr, "%d: %s\n", errno, strerror(errno));
    return -1;
  }

  if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
#ifdef ES2
    dirwalk(argv[1], invsymlink, false);
#else
    fprintf(stderr, "Ma questa e' una cartella!\n";
#endif
  else {
    if (argv[1][0] == '/')
      invsymlink(argv[1]);
    else {
      char path[MAX_PATH];
      getcwd(path, MAX_PATH);
      if (strlen(path) + strlen(argv[1]) + 2 > sizeof(path))
        fprintf(stderr, "main: il nome %s/%s e' troppo lungo\n", path, argv[1]);
      else
        sprintf(path, "%s/%s", path, argv[1]);
      invsymlink(path);
    }
  }
}

#ifdef ES2
void dirwalk(const char *dir, void (*fcn)(char *), bool recursive) {
  char path[MAX_CHILDREN][MAX_PATH];
  bool isDir[MAX_CHILDREN];
  struct dirent *dp;
  DIR *dfd;
  char *const absdir = realpath(dir, NULL);
  // printf("%s\n", absdir);
  if ((dfd = opendir(dir)) == NULL) {
    fprintf(stderr, "dirwalk: non riesco ad aprire %s\n", dir);
    return;
  }
  int children;
  for (children = 0; children < MAX_CHILDREN && (dp = readdir(dfd)) != NULL;
       ++children) {
    struct stat stbuf;
    chdir(dir);
    if (lstat(dp->d_name, &stbuf) == -1) {
      fprintf(stderr, "dirwalk: non riesco ad aprire %s\n", dp->d_name);
      exit(-1);
    }
    if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") ||
        !S_ISLNK(stbuf.st_mode)) {
      --children;
      continue;
    }

    if (strlen(absdir) + strlen(dp->d_name) + 2 > sizeof(*path))
      fprintf(stderr, "dirwalk: il nome %s/%s e' troppo lungo\n", absdir,
              dp->d_name);
    else {
      sprintf(path[children], "%s/%s", absdir, dp->d_name);
    }
    isDir[children] = dp->d_type == DT_DIR;
  }
  for (int i = 0; i < children; ++i)
    if (isDir[i] && recursive)
      dirwalk(path[i], fcn, true);
    else {
      // printf("%s\n", path[i]);
      (*fcn)(path[i]);
    }
  closedir(dfd);
  free(absdir);
}
#endif

void invsymlink(char *path) {
  struct stat stbuf;

  if (lstat(path, &stbuf) == -1) {
    fprintf(stderr, "invsymlink: non riesco ad aprire %s\n", path);
    exit(-1);
  }
  if (!S_ISLNK(stbuf.st_mode))
    return;

  char *const rp = realpath(path, NULL);
  // printf("%s VS %s\n", path, rp);

  if (remove(path))
    fprintf(stderr, "remove failed!\n");
  if (rename(rp, path))
    fprintf(stderr, "rename failed!\n");
  if (symlink(path, rp))
    fprintf(stderr, "symlink failed!\n");
  free(rp);
}
