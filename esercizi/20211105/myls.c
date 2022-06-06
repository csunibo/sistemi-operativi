#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

int main (int argc, char *argv[]) {
	DIR *d = opendir(argv[1]);
	struct dirent *de;
	while ((de = readdir(d)) != NULL)
		printf("%s\n", de->d_name);
	closedir(d);
}
