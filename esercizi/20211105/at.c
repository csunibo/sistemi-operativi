#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
	int dirfd = open("/etc", O_PATH);
	int fd = openat(dirfd, "hostname", O_RDONLY);
	char buf[100];
	int n=read(fd, buf, sizeof(buf));
	write(1, buf, n);
}
