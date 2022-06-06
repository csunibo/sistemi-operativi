#include <unistd.h>
#include <stdio.h>
int main(int argc, char *argv[]){
	char* newargv[] = {"hello", NULL};
	printf("%d\n", getpid());
	execve("./hello",newargv, NULL);
	return 0;
}
