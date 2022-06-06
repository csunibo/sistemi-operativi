#include <unistd.h>
#include <stdio.h>
int main(int argc, char *argv[], char* env[]){
	char* newargv[] = {"hello", NULL};
	for(;env!=NULL;env++){
		printf("%s\n", env);
	}
	printf("%d\n", getpid());
	execve("./hello",newargv, NULL);
	return 0;
}
