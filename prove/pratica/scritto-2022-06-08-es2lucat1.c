/* NOTA: la funzionalita' di deduplicazione dello scambio dei link e'
 * implementata, tuttavia, richiede che il parametro in input venga passato
 * senza la '/' in fondo.
 */
#include <dirent.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define INITIAL 4

#define err(e)                                                                 \
  do {                                                                         \
    perror(e);                                                                 \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

char *mem;
size_t allo = INITIAL, n = 0;

void swap(const char *in, bool graceful) {
  int realfd, destfd;
  struct stat s;
  char real[PATH_MAX];

  if (lstat(in, &s) == -1)
    err("lstat");

  if (!S_ISLNK(s.st_mode)) {
    if (graceful)
      return;
    fprintf(stderr, "%s is not a symlink\n", in);
    exit(EXIT_FAILURE);
  }

  printf("swapping %s\n", in);
  if (realpath(in, real) == NULL)
    err("realpath");

  if ((realfd = open(real, O_RDONLY)) == -1)
    err("open/real");

  if (stat(real, &s) == -1)
    err("stat");

  if (unlink(in) == -1)
    err("unlink");

  if ((destfd = open(in, O_WRONLY | O_CREAT, 0666)) == -1)
    err("open/dest");

  if (sendfile(destfd, realfd, 0, s.st_size) == -1)
    err("sendfile");

  if (unlink(real) == -1)
    err("unlink");

  if (symlink(in, real) == -1)
    err("symlink");

  if (n >= allo) {
    allo *= 2;
    if ((mem = realloc(mem, PATH_MAX * allo)) == NULL)
      perror("realloc");
  }
  strcpy((char *)(mem + (n * PATH_MAX)), real);
  ++n;

  close(realfd);
  close(destfd);
}

void dir(const char *in) {
  DIR *d;
  struct dirent *e;
  char merged[PATH_MAX];

  if ((d = opendir(in)) == NULL)
    err("opendir");

  if ((mem = (char *)malloc(PATH_MAX * allo)) == NULL)
    perror("malloc");
  while ((e = readdir(d)) != NULL) {
    if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, ".."))
      continue;

    snprintf(merged, PATH_MAX, "%s/%s", in, e->d_name);
    bool already = false;
    for (size_t i = 0; i < n && !already; ++i) {
      char *ele = mem + (i * PATH_MAX);
      if (!strcmp(ele, merged))
        already = true;
    }

    if (!already)
      swap(merged, true);
  }
  free(mem);
  closedir(d);
}

int main(int argc, char *argv[]) {
  struct stat s;
  char cwd[PATH_MAX], in[PATH_MAX];
  if (argc != 2) {
    printf("Usage: %s [dir]", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (getcwd(cwd, PATH_MAX) == NULL)
    err("getcwd");

  /* Absolute paths are required here as the new link will have to point to the
   * approriate file even if they aren't relative to one another. Example:
   * A/B/file
   * C/D/link -> ../../A/B/file
   *
   * Inside of C/ I run invsymlink and therefore the argv[1] will be "D/link".
   * This means that unless the path is conveted to absolute the new link
   * willdon point to "D/link" which cannot be resolved relative to A/B/
   * */
  snprintf(in, PATH_MAX, "%s/%s", cwd, argv[1]);

  if (lstat(in, &s) == -1)
    err("lstat");

  if (stat(in, &s) == -1)
    err("stat");

  if (S_ISDIR(s.st_mode))
    dir(in);
  else
    swap(in, false);

  return 0;
}
