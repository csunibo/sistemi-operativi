#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char *argv[]){
	struct stat buf;
	int fin = open(argv[1], O_RDONLY);
	fstat(fin, &buf);
	printf("%d %d\n", buf.st_uid, buf.st_gid);
	switch (buf.st_mode & S_IFMT) {
		case S_IFBLK:  printf("block device\n");            break;
		case S_IFCHR:  printf("character device\n");        break;
		case S_IFDIR:  printf("directory\n");               break;
		case S_IFIFO:  printf("FIFO/pipe\n");               break;
		case S_IFLNK:  printf("symlink\n");                 break;
		case S_IFREG:  printf("regular file\n");            break;
		case S_IFSOCK: printf("socket\n");                  break;
		default:       printf("unknown?\n");                break;
	}
	close (fin);
}
