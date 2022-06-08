#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define err(e)                                                                 \
  do {                                                                         \
    perror(e);                                                                 \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

int main(int argc, char *argv[]) {
  int realfd, destfd;
  struct stat s;
  char real[PATH_MAX], cwd[PATH_MAX], in[PATH_MAX];

  if (argc != 2) {
    printf("Usage: %s [file]", argv[0]);
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

  if (!S_ISLNK(s.st_mode)) {
    fprintf(stderr, "%s is not a symlink\n", in);
    exit(EXIT_FAILURE);
  }

  if (realpath(in, real) == NULL)
    err("realpath/real");

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

  close(realfd);
  close(destfd);

  return 0;
}
