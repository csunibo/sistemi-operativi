#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */

int main (int argc, char * argv[]){
	//printf ("hello world\n");
	//write (1, "hello world\n", 12);
	syscall(__NR_write, 1, "hello world\n", 12);
}
