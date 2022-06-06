#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <dirent.h>
#include <stdint.h>
struct linux_dirent64 {
	uint64_t		d_ino;
	int64_t		d_off;
	unsigned short	d_reclen;
	unsigned char	d_type;
	char		d_name[];
};


#define DBUFSIZE 1024
int main (int argc, char *argv[]) {
	int fd = open(argv[1], O_RDONLY | O_DIRECTORY);
	char buf[DBUFSIZE];
	ssize_t len;
	while ((len = syscall(__NR_getdents64, fd, buf, DBUFSIZE)) > 0) {
		char *pos;
		size_t reclen = 0;
		for (pos = buf; pos < buf + len; pos += reclen) {
			struct linux_dirent64 *de = (struct linux_dirent64 *) pos;
			printf("%ld   %s\n", de->d_ino, de->d_name);
			reclen = de->d_reclen;
		}
	}
	close(fd);
}
