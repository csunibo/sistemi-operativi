#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFLEN 8192

int main(int argc, char *argv[]){
	char buf[BUFLEN];
	ssize_t n;
	int fin = open(argv[1], O_RDONLY);
	int fout = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666);
	while((n = read(fin, buf, BUFLEN)) > 0){
		write (fout, buf, n);
	}
	close (fin);
	close (fout);
}
