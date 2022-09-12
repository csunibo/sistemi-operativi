#include "davolib.h"
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

 void main(){
 	string_t *str = create_string("192.168.0.1");
 	int len = substring_occurrences(str, ".") +1;
 	char *tokens[len];
 	split_string(str, ".", tokens);
 	for (int i = 0; i < len; i++){
 		printf("%s\n", *(tokens+i));
    }

    //char contenuto[getFileSize("./dddd") +1];
    //init_array(contenuto);
    //readWholeFile("./dddd", contenuto);
    //printf("%s\n", contenuto);

    //appendFileToFile("./mondo.txt", "./ciao.txt");


    /*
            fileSize = getFileSize(newFilePath->data);
            printf("%d", fileSize);
            char *fileContentBuf = malloc(fileSize * (sizeof(char)) + 1);
            fileContentBuf[fileSize] = '\0';
            readWholeFile(newFilePath->data, fileContentBuf);

            fwrite(fileContentBuf, sizeof(char), fileSize, logFile); //da buffer a stream

            //libero le risorse e aspetto il prossimo evento
            free(fileContentBuf);
            */
    /*
    const char *vector[3] = {"/bin/cat", "/home/matteo/Desktop/test.cpp", NULL};
    char output[120];
    init_array(output);
    getOutput(vector[0], vector, output, 120);
    printf("ottenudto: %s\n", output);
    printf("sono arrivato qui!\n");
   */
    char *contenutoStringato;
    //https://stackoverflow.com/questions/14992772/modifying-string-literal-passed-in-as-a-function
    int lunghezza = readWholeFile("./davolib.h", &contenutoStringato);
    printf("\nsono pesante %d (compreso \'\\0\') e contengo:\n%s\n mia strlen (sarà pesante-1): %d\n", lunghezza, contenutoStringato, strlen(contenutoStringato));
 }