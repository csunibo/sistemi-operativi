/* https://linuxhint.com/dup2_system_call_c/ */
/* https://linuxconfig.org/cat-command#:~:text=When%20the%20cat%20command%20does,by%20CTRL%2BD%20key%20combination. */
/* https://stackoverflow.com/questions/35823864/detecting-ctrl-d-in-c */
/* https://stackoverflow.com/questions/14001907/read-data-from-file-till-end-of-line-in-c-c */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
int main(){
    char buf[64]; bzero(buf, sizeof(buf));
    while (fgets(buf, sizeof(buf), stdin));
    printf("MAN HAI FATTO CTRL D!");
    exit(EXIT_SUCCESS);
}
