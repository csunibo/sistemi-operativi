#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int run(const char * cmd) {
	char *newargv[100];
	int newargc;
	char *scan;
	int status;
	char cmdcopy[strlen(cmd) +1];
	strcpy (cmdcopy, cmd);
	for (scan = cmdcopy, newargc=0; 1; newargc++, scan = NULL) {
		if ((newargv[newargc] = strtok(scan, " \t")) == NULL)
			break;
  }
	switch (fork()) {
		case 0:
			execvp(newargv[0], newargv);
			exit(1);
		default:
			wait(&status);
			break;
		case -1:
			exit(1);
	}
}
	static void
handle_events(int fd, int wd, int argc, char* argv[])
{
	char buf[4096]
		__attribute__ ((aligned(__alignof__(struct inotify_event))));
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

			event = (const struct inotify_event *) ptr;

			if (event->mask & IN_CREATE) {
				printf("IN_CREATE: %s\n", event->name);
				run(event->name);
				unlink(event->name);
			}
		}
	}
}

	int
main(int argc, char* argv[])
{
	char buf;
	int fd, i, poll_num;
	int wd;
	nfds_t nfds;
	struct pollfd fds[2];

	if (argc!=2) {
		printf("Usage: %s PATH \n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = inotify_init1(0);
	if (fd == -1) {
		perror("inotify_init1");
		exit(EXIT_FAILURE);
	}
	chdir(argv[1]);
	wd = inotify_add_watch(fd, ".", IN_CREATE);
	if (wd == -1) {
		fprintf(stderr, "Cannot watch '%s': %s\n",
				argv[i], strerror(errno));
		exit(EXIT_FAILURE);
	}
	handle_events(fd, wd, argc, argv);

	close(fd);
	exit(EXIT_SUCCESS);
}
