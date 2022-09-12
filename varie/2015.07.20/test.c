/* http://sticksandstones.kstrom.com/appen.html */
#include <stdio.h>
#define HANDLE_ERROR(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
#include <sys/stat.h>
/* https://stackoverflow.com/questions/16254497/c-read-and-replace-char */ //IMPORTANTISSIMO !!!
char reverse_char(char c){
    int n = sizeof(char)*8;
    /* int n = 8; */
    //converting to binary
    int bin[n]; INIT_ARRAY(bin);
    for (int i = 0; i < n; i++){
        bin[i] = (c >> i) & 1;
    }
    //reverse array
    for(int i = i; i < n/2; i++){
        int tmp = bin[i];
        bin[i] = bin[n-i-1];
        bin[n-i-1] = tmp;
    }
    char reversed_str_bin[n]; INIT_ARRAY(reversed_str_bin);
    for (int i=0; i < n; i++){
        /* https://stackoverflow.com/questions/30234363/how-can-i-convert-an-int-array-into-a-string-array */
        sprintf(&reversed_str_bin[i], "%d", bin[i]);
    }
    /* https://stackoverflow.com/questions/10825218/convert-from-a-binary-to-char-in-c */
    char reversed_char = strtol(reversed_str_bin, (char **)NULL, 2);
    return reversed_char;
}
int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "BAD USAGE\n");
        exit(EXIT_FAILURE);
    }

    FILE *stream = fopen(argv[1], "rb+"); //reading and writing
    if (stream == NULL) HANDLE_ERROR("fopen()\n");
    rewind(stream);
    struct stat buf; bzero(&buf, sizeof(struct stat));
    if (stat(argv[1], &buf) == -1) HANDLE_ERROR("stat()\n");
    int i = 0;
    while (!feof(stream)){
        fpos_t pos;
        fgetpos(stream, &pos);
        char buf = fgetc(stream);
        if (buf == EOF) HANDLE_ERROR("fgetc");
        fsetpos(stream, &pos);
        //sizeof(char) mi dà 2
        if (fputc(reverse_char(buf), stream) == EOF) HANDLE_ERROR("fputc");
        if (fflush(stream) == EOF) HANDLE_ERROR("fflush");
        /* When a file is opened with update mode ('+' as the second or third character in the above list of mode argument values), both input and output may be performed on the associated stream. */

        /*     However, output shall not be directly followed by input without an intervening call to the fflush function or to a file positioning function (fseek, fsetpos, or rewind), and input shall not be directly followed by output without an intervening call to a file positioning function, unless the input operation encounters end-of-file. */
        fseek(stream, (long)++i, SEEK_SET);
    }
    fclose(stream);
    exit(EXIT_SUCCESS);
}
