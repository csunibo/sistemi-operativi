// Lo scopo di questo esercizioè di scrivere msleep. msleep si comporta come
// sleep(1) ma invece di attendere il numero di secondi indicati come parametro
// deve aspettare il numero di millisecondi indicati come parametro.d es: msleep
// 2340 completa la sua esecuzione in 2340 millisecondi alias 2.34 secondi. La
// msleep deve essere implementata usando i timerfd (v. timerfd_create(2)).

#include <errno.h>
#include <inttypes.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timerfd.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  struct pollfd *pfds;
  size_t r;
  uint16_t exp;
  struct timespec now;

  if (argc < 2) {
    printf("Usage: %s time...\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  size_t o = argc - 1;
  pfds = calloc(o, sizeof(struct pollfd));
  if (pfds == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  if (clock_gettime(CLOCK_REALTIME, &now) == -1) {
    perror("timerfd_create");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < o; ++i) {
    char *endptr, *str = argv[i + 1];
    uint64_t time = (uint64_t)strtol(str, &endptr, 10);
    if (errno != 0 || endptr == str) {
      perror("strol");
      exit(EXIT_FAILURE);
    }

    if ((pfds[i].fd = timerfd_create(CLOCK_REALTIME, 0)) == -1) {
      perror("timerfd_create");
      exit(EXIT_FAILURE);
    }
    struct itimerspec new_value;
    new_value.it_value.tv_nsec = (now.tv_nsec + time * 1000000) % 1000000000;
    new_value.it_value.tv_sec = (now.tv_sec + time / 1000) +
                                ((now.tv_nsec + time * 1000000) / 1000000000);
    // expire only once
    new_value.it_interval.tv_nsec = new_value.it_interval.tv_sec = 0;
    errno = 0;
    if (timerfd_settime(pfds[i].fd, TFD_TIMER_ABSTIME, &new_value, NULL) ==
        -1) {
      perror("timerfd_settime");
      exit(EXIT_FAILURE);
    }
    pfds[i].events = POLLIN | POLLHUP | POLLERR;
  }

  size_t waiting = o;
  while (waiting > 0) {
    int ready = poll(pfds, o, -1);
    if (ready == -1) {
      perror("poll");
      exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < o; ++i) {
      if (pfds[i].revents == 0)
        continue;

      if (pfds[i].revents & POLLIN) {
        errno = 0;
        if ((r = read(pfds[i].fd, &exp, sizeof(uint64_t))) !=
            sizeof(uint64_t)) {
          perror("read");
          exit(EXIT_FAILURE);
        }
        printf("%s\n", argv[i + 1]);
        --waiting;
      } else {
        if (close(pfds[i].fd) == -1) {
          perror("close");
          exit(EXIT_FAILURE);
        }
        --o;
      }
    }
  }

  free(pfds);
  return 0;
}
