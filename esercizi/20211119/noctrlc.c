#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static void sigh(int signo) {
	printf("prrr\n");
}

int main(int argc, char * argv[]) {
	signal(SIGINT, sigh);
	for (int i = 0; 1 ; i++) {
		sleep(2);
		printf("tran tran %d\n", i);
	}
}
