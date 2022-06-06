#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char *argv[]){
	int fin = open(argv[1], O_WRONLY|O_TRUNC|O_CREAT, 0666);
	write(fin, "ciao", 4);
	off_t pos = lseek(fin, 0, SEEK_CUR); 
	printf("%zd\n", pos);
	off_t pos1 = lseek(fin, 1000000, SEEK_SET);
	printf("%zd\n", pos1);
	write(fin, "mare", 4);
	close (fin);
}
