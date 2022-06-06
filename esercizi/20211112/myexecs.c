#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 

int myexecsp(const char *cmd) {
	size_t cmdlen = strlen(cmd) + 1;
	char cmdcpy[cmdlen];
	strcpy(cmdcpy, cmd);
	int argc;
	char *cmdtmp;
	char *saveptr;
	for (cmdtmp = cmdcpy, argc = 0; 
			strtok_r(cmdtmp, " \t", &saveptr);
			argc++, cmdtmp = NULL)
		;
	char *argv[argc + 1];
	strcpy(cmdcpy, cmd);
	for (cmdtmp = cmdcpy, argc = 0; 
			argv[argc] = strtok_r(cmdtmp, " \t", &saveptr);
			argc++, cmdtmp = NULL)
		;
	return execvp(argv[0], argv);
}

int main(int argc, char *argv[]) {
	return myexecsp(argv[1]);
}
