#include <stdio.h>
#include <ctype.h>

int main(int argc, char * argv[]){
    FILE *input;
    input = fopen(argv[1], "rb+");
    if (!input){
        fprintf(stderr, "Unable to open file %s", argv[1]);
        return -1;
    }

    int ch;
    fpos_t pos, pos_end;
    fgetpos(input, &pos);
    fseek(input, 0L, SEEK_END);
    fgetpos(input, &pos_end);
    rewind(input);
    while(pos != pos_end){
        ch=fgetc(input);
        if(EOF==ch)break;
        printf("%c",ch);
        if(!iscntrl(ch) && !iscntrl(ch+1)){
            fsetpos(input, &pos);
            fputc(ch+1, input);
            fflush(input);
        }
        pos += 1;
        fsetpos(input, &pos);
    }
    fclose(input);
    return 1;
}
