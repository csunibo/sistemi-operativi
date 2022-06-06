
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
	int dirfd = open(".", O_PATH);
	printf("%s\n", getcwd(NULL, 0));
	chdir("/etc");
	printf("%s\n", getcwd(NULL, 0));
	fchdir(dirfd);
	printf("%s\n", getcwd(NULL, 0));
}
