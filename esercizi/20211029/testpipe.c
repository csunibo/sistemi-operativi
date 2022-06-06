#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
	int pfd[2];
	pipe(pfd);
	char *str = "Hello World!";
	char buf[128];
	write(pfd[1], str, strlen(str)+1);
	read(pfd[0], buf, sizeof(buf));
	printf("%s\n", buf);
}
