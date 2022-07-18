#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void sig_handler(int sig, siginfo_t *info, void *ucontext){
	union sigval value = info->si_value;
	int64_t tmp = (int64_t)value.sival_ptr;
	char converted[8];
	for(int i=0; i<8; i++){
		converted[i]=tmp%256;
		tmp/=256;
	}
	printf("RECEIVED: %s\n", converted);
}


int main(int argc, char* argv[]){
	pid_t pid = getpid();
	printf("My pid: %d\n", (int)pid);
	
	
	struct sigaction sigact;
	sigact.sa_flags = SA_SIGINFO;
	sigact.sa_sigaction = sig_handler;
	while(1){
		int ret = sigaction(SIGUSR1, &sigact, NULL);
	}
	
	
	return 0;
}
