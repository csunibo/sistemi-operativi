#include <errno.h>
#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define LEN 512

int readall(FILE *src, FILE *dest) {
	char buf[LEN];
	while (fgets(buf, LEN, src) != NULL)
		if (fputs(buf, dest) == EOF)
			return 1;
	return 0;
}

bool exec(FILE *f, char *fp) {
	pid_t p;
	switch ((p = fork())) {
		case -1:
			perror("fork");
			return false;
		case 0:
			fflush(f);
			dup2(fileno(f), 1); // dup stdout
			dup2(fileno(f), 2); // dup stderr
			execlp(fp, fp);
			break;
		default:;
			pid_t res = waitpid(p, NULL, 0);
			printf("done %d\n", p);
			fflush(f);
			return res;
	}
	// make gcc happy
	return false;
}

static void handle_events(int fd, int wd, char *dir, FILE *f) {
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

			/* Print event type */

			if (event->len && (event->mask & IN_ISDIR) || !(event->mask & IN_CLOSE))
				break;

			bool addSlash = dir[strlen(dir) - 1] != '/';
			char *s = (char *)malloc((strlen(dir) + event->len + (addSlash ? 1 : 0)) *
					sizeof(char));
			strncpy(s, dir, strlen(dir));
			if (addSlash)
				s[strlen(dir)] = '/';
			strncpy(s + strlen(dir) + (addSlash ? 1 : 0), event->name,
					event->len); // copy trailing \0
			fprintf(f, "(%s)\n", s);
			if (event->len)
				printf("! (%s)\n", event->name);

			struct stat st;
			if (stat(s, &st)) {
				perror("stat");
				exit(EXIT_FAILURE);
			}
			if (st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
				if (!exec(f, s)) {
					perror("ececlp");
					exit(EXIT_FAILURE);
				}
			} else {
				FILE *src = fopen(s, "r");
				if (src == NULL) {
					perror("fopen");
					exit(EXIT_FAILURE);
				}

				if (readall(src, f))
					exit(EXIT_FAILURE);
				fclose(src);
			}
			fprintf(f, "\n");
			if (unlink(s)) {
				perror("unlink");
				exit(EXIT_FAILURE);
			}
			free(s);
		}
	}
}

int main(int argc, char *argv[]) {
	char buf;
	int fd, i, poll_num;
	int wd;
	nfds_t nfds;
	FILE *f;
	struct pollfd fds[2];

	if (argc < 3) {
		printf("Usage: %s DIR OUT\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	printf("Press ENTER key to terminate.\n");

	/* Create the file descriptor for accessing the inotify API */

	fd = inotify_init1(IN_NONBLOCK);
	if (fd == -1) {
		perror("inotify_init1");
		exit(EXIT_FAILURE);
	}

	f = fopen(argv[2], "a");
	if (f == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	wd = inotify_add_watch(fd, argv[1],
			/* IN_OPEN | IN_CLOSE | IN_CREATE */ IN_ALL_EVENTS);
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

		if (poll_num > 0) {

			if (fds[0].revents & POLLIN) {

				/* Console input is available. Empty stdin and quit */

				while (read(STDIN_FILENO, &buf, 1) > 0 && buf != '\n')
					continue;
				break;
			}

			if (fds[1].revents & POLLIN) {

				/* Inotify events are available */

				handle_events(fd, wd, argv[1], f);
			}
		}
	}

	printf("Listening for events stopped.\n");

	close(fd);
	fflush(f);
	fclose(f);

	exit(EXIT_SUCCESS);
}
