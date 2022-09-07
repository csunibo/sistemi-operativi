#include <asm-generic/errno-base.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

/* https://stackoverflow.com/questions/9067175/how-to-monitor-a-folder-with-all-subfolders-and-files-inside */
#define BUFFER_SIZE 1024

int cpy(FILE *src, FILE *dest) {
    char buf[BUFFER_SIZE];
    size_t read;
    while((read = fread(buf, sizeof(char), BUFFER_SIZE, src))) {
        if(fwrite(buf, sizeof(char), read, dest) != read) return 1;
    }
    fclose(src);
    fclose(dest);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("\nBAD USAGE");
        exit(EXIT_FAILURE);
    }
    int fdToWatch = open(argv[1], O_RDONLY);

    if (mkdir("tmp", 0777) != 0){
        if (errno != EEXIST){
            perror("\nmkdir");
            exit(EXIT_FAILURE);
        }
    }

    //parte iniziale dove copio i file iniziali. Se tramite inotofy file creato => stesso procedimento. Se tramite inotofy file eliminato => inverto ruoli
    int fdTmp = open("tmp", O_RDONLY);
    if(fdTmp == -1){
        perror("fdTmp");
        exit(EXIT_FAILURE);
    }

    DIR *dir = fdopendir(fdToWatch);
    if (dir == NULL){
        perror("fdopendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while((entry = readdir(dir)) != NULL){
        if (entry->d_type == DT_REG) {
            /* https://stackoverflow.com/questions/47331344/system-call-open-c-cant-print-file-content-if-the-file-is-not-opened-as-sudo */
            int fd = openat(fdTmp, entry->d_name, O_WRONLY| O_RDONLY | O_CREAT, 0777);
            if (fd == -1){
                perror("openat");
                exit(EXIT_FAILURE);
            }
            //copy file content
            char path[PATH_MAX];
            memset(path, 0, sizeof(path));
            snprintf(path, PATH_MAX, "%s/%s", argv[1], entry->d_name);
            FILE *stream = fopen(path, "rb");
            if (stream == NULL){
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            cpy(stream, fdopen(fd, "wr"));
        }
    }
    return 0;
}
