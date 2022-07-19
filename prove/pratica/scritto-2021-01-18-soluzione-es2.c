#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

int copyfile(char* original, char* copy){
	int fd_orig, fd_cpy;
	fd_orig = open(original, O_RDONLY);
	if(fd_orig == -1){
		printf("ERR on reading original file!\n");
		return -1;
	}
	fd_cpy = open(copy, O_WRONLY | O_CREAT, 0666);
	struct stat s;
	lstat(original, &s);
	
	
	
	sendfile(fd_cpy,fd_orig,0,s.st_size);
	close(fd_orig);
	close(fd_cpy);
	return 0;	
}

int main(int argc, char* argv[]){
	if(argc != 2){	//controllo che il numero di parametri sia corretto
		printf("ERR! Usage: %s PATH_TO_FOLDER\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	struct stat s;
	lstat(argv[1], &s);
	if(S_ISDIR(s.st_mode)){
		DIR *d = opendir(argv[1]);
		struct dirent *e;
		while((e=readdir(d))!=NULL){
			int fd = open(e->d_name, O_RDONLY);
			if(fd != -1){
				struct stat s;
				lstat(e->d_name,&s);
				if(S_ISLNK(s.st_mode)){	//se il file in questione è effettiamente un link
					char real[1024];
					realpath(e->d_name, real);
					printf("Real path of %s: %s\n",e->d_name,real);
					close(fd);
					char cpyname[1024];
					strcpy(cpyname, e->d_name);
					unlink(e->d_name);
					copyfile(real,cpyname);
				}else{
					close(fd);
				}
			}
		}
	}else{
		printf("ERR! Not a directory\n");
		exit(EXIT_FAILURE);
	}
	
		
	return 0;
}

