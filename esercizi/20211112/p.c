#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char * argv[]) {
	char *newargv[100];
	int newargc;

	char *command = argv[1];
	char *scan;
	for (scan = command, newargc=0; 1  ; newargc++, scan = NULL) {
		if ((newargv[newargc] = strtok(scan, " \t")) == NULL)
			break;
  }

	int i;
	for (i=0; i<newargc; i++) 
		printf("%d %s\n",i, newargv[i]);

	execvp(newargv[0], newargv);
}
