#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#define INIT_ARRAY(T)(  bzero((T), sizeof((T))) )
void reverse_bin(char arg){
    /* char *bin = (char *)malloc(sizeof(char)*8 + 1); */
    char c = arg;
    for (int i=0; i < sizeof(char); i++){ //for each byte
        char byte = *( (char *)&c + i ); //((char *)&c)[i] 13 => 1101
        for (int j = 0; j < 8; j++){ //for each bit
            char bit = byte & 1;
            byte >>= 1;
            printf("%hhd", bit);
        }
    }
}

/* "Address of stack memory associated with local variable returned => malloc" */
/* https://stackoverflow.com/questions/18041100/using-c-string-gives-warning-address-of-stack-memory-associated-with-local-var */
char *char_to_bin_str_mod(char arg){
    char c = arg;
    unsigned int n = sizeof(char)*8;
    char *bin = (char *)malloc(n+1); //bzero(bin, n+1); //NULL terminated
    for (int i=0; i<n; i++){
        bin[i] = c%2; //dal meno significativo
        c = c/2;
    }
    return bin;
}

char *char_to_bin_str_shift(unsigned char arg){
    unsigned char c = arg;
    unsigned int n = sizeof(char)*8;
    char *bin = (char *)malloc(n+1);
    for (int i=0; i<n; i++){
        bin[i] = (c >> i) & 1; //least significant bit first, partendo dal fondo
    }
    return bin;
}

int main(int argc, char **argv){
    char_to_bin_str('a');
    pause();
    unsigned int n = sizeof(char)*8;
    char *bin = char_to_bin_str_shift('a');
    char converted = strtol(bin, (char **)NULL, 2); //gli passiamo una stringa
    printf("converted: %c\n", converted);
    free(bin);
    exit(EXIT_SUCCESS);
}
