#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dlfcn.h>

int main(int argc, char* argv[]){
	if(argc < 2){	//controllo che il numero di parametri sia corretto
		printf("ERR! Usage: %s PATH_TO_FOLDER [ARGS]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	char path[1024];
	char buf[1024];
	getcwd(buf,sizeof(buf));
	sprintf(path,"%s/%s",buf,argv[1]);
	
	void *tmp;
	tmp = dlopen(path,RTLD_LAZY);
	if(tmp == NULL){
		printf("Err in dlopen\n");
		exit(EXIT_FAILURE);
	}
	

	void *fun = dlsym(tmp,"main");
	int ret = ((int(*)(int,char*))fun)(argc-1,argv+1);
	//printf("\nRET=%d\n",ret);
	
	dlclose(tmp);
	
	return 0;
}
