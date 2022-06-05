#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void join(char *abs, const char *p1, const char *p2) {
  bool ending_slash = p1[strlen(p1) - 1] == '/';
  strncpy(abs, p1, strlen(p1));
  if (!ending_slash)
    strncpy(abs + strlen(p1), "/", 1);
  strncpy(abs + strlen(p1) + (ending_slash ? 0 : 1), p2, strlen(p2));
}

static void handle_events(int fd, int wd, char *dir) {
  char buf[4096] __attribute__((aligned(__alignof__(struct inotify_event))));
  const struct inotify_event *event;
  ssize_t len;

  /* Loop while events can be read from inotify file descriptor. */

  for (;;) {

    /* Read some events. */

    len = read(fd, buf, sizeof(buf));
    if (len == -1 && errno != EAGAIN) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    /* If the nonblocking read() found no events to read, then
                  it returns -1 with errno set to EAGAIN. In that case,
                  we exit the loop. */

    if (len <= 0)
      break;

    /* Loop over all events in the buffer */

    for (char *ptr = buf; ptr < buf + len;
         ptr += sizeof(struct inotify_event) + event->len) {
      event = (const struct inotify_event *)ptr;
      if (event->mask & IN_ISDIR)
        continue;
      /* Print the name of the watched directory */
      char abs[PATH_MAX];

      join(abs, dir, event->name);
      if (event->mask & IN_DELETE) {
        printf("- %s\n", abs);

        int fd = open(abs, O_CREAT, 0666);
        if (fd == -1) {
          perror("open");
          exit(EXIT_FAILURE);
        }
        if (close(fd) == -1) {
          perror("close");
          exit(EXIT_FAILURE);
        }
      }
    }
  }
}

int main(int argc, char *argv[]) {
  char buf;
  int fd, i, poll_num;
  int wd;
  nfds_t nfds;
  struct pollfd fds[2];

  if (argc != 2) {
    printf("Usage: %s PATH\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  printf("Press ENTER key to terminate.\n");

  /* Create the file descriptor for accessing the inotify API */

  fd = inotify_init1(IN_NONBLOCK);
  if (fd == -1) {
    perror("inotify_init1");
    exit(EXIT_FAILURE);
  }

  wd = inotify_add_watch(fd, argv[1], IN_DELETE | IN_CLOSE);
  if (wd == -1) {
    fprintf(stderr, "Cannot watch '%s': %s\n", argv[1], strerror(errno));
    exit(EXIT_FAILURE);
  }

  /* Prepare for polling */

  nfds = 2;

  /* Console input */

  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;

  /* Inotify input */

  fds[1].fd = fd;
  fds[1].events = POLLIN;

  /* Wait for events and/or terminal input */

  printf("Listening for events.\n");
  while (1) {
    poll_num = poll(fds, nfds, -1);
    if (poll_num == -1) {
      if (errno == EINTR)
        continue;
      perror("poll");
      exit(EXIT_FAILURE);
    }

    if (poll_num <= 0)
      continue;

    if (fds[0].revents & POLLIN) {
      /* Console input is available. Empty stdin and quit */
      while (read(STDIN_FILENO, &buf, 1) > 0 && buf != '\n')
        continue;
      break;
    }

    if (fds[1].revents & POLLIN)
      handle_events(fd, wd, argv[1]);
  }

  printf("Listening for events stopped.\n");

  close(fd);
  exit(EXIT_SUCCESS);
}
