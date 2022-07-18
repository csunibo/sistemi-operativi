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

int64_t char8_to_int64(char *s){
	return *((int64_t*)s);
}


int main(int argc, char* argv[]){
	if(argc != 3){
		printf("ERR! Usage: %s PID 8_CHAR_MSG\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	pid_t pid = (pid_t)atoi(argv[1]);
	int64_t msg_conv = char8_to_int64(argv[2]); 
	
	union sigval value;
	value.sival_ptr = (void*)msg_conv;
	int res = sigqueue(pid,SIGUSR1,value);
	//printf("Trying to send %s which is %ld to %ld\n",argv[2], msg_conv, pid);
	if(res != 0){
		printf("ERR ON SIGQUEUE!\n");
		exit(EXIT_FAILURE);
	}
	
	
	return 0;
}
