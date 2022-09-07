#include <unistd.h>
#include <strings.h>
#include <stdio.h>
int main(){
    dup2(STDOUT_FILENO, STDIN_FILENO);
    /* close(STDOUT_FILENO); */
    while (1){
        char buf[64]; bzero(buf, sizeof(buf));
        read(STDIN_FILENO, buf, sizeof(buf));
        printf("%s\n", buf);
    }
}
