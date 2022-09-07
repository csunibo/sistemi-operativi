/*
Il comando abslink, da implementare, deve sostituire un link simbolico con uno equivalente che sia
un riferimento ad un path assoluto.
Ad esempio: se mylink e' nella directory /home/user e punta a myfile, 'abspath mylink" deve sostituire
mylink con un symbolic link a /home/user/myfile.
Hint: considerate l'uso della funzione realpath.
*/
//da soft link (collegamento desktop) con path relativa a soft link con path assoluta (soft = symbolic)
//If there is no error, realpath() returns a pointer to the resolved_path.
//https://github.com/AngeloGalav/UniOSScripts/blob/main/2020.02.21/es1/abspath.c
//https://stackoverflow.com/questions/1951742/how-can-i-symlink-a-file-in-linux
//right-click => properties => link target
#include <stdio.h>
#include <limits.h> /* PATH_MAX */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//abspath mylink
int main(int argc, char** argv){
    char buf[PATH_MAX]; /* PATH_MAX incudes the \0 so +1 is not required */
    char *res = realpath(argv[1], buf);
    if (res == NULL) printf("file not found.\n");
    else {
        //unlink(argv[1]); /* uncomment this line to replace the original link */
        symlink(res, strcat(argv[1], "_new")); // 0 if success
    }
}