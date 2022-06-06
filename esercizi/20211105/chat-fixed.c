#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

int main (int argc, char *argv[]) {
	int fdin = open(argv[1], O_RDONLY | O_NONBLOCK);
	int fdout = open(argv[2], O_WRONLY);

	struct pollfd pfd[] = {{STDIN_FILENO, POLLIN}, 
		{fdin, POLLIN}};

	while (1) {
		int n = poll(pfd, 2, -1);
		if (pfd[0].revents) {
			char buf[1024];
			int len = read(STDIN_FILENO, buf, 1024);
			if (len == 0) break;
			write(fdout, buf, len);
		}
		if (pfd[1].revents) {
			char buf[1024];
			int len = read(fdin, buf, 1024);
			if (len == 0) break;
			write(STDOUT_FILENO, buf, len);
		}
	}



}
