#include <stdio.h>
#include <stdlib.h>
char reverse_char(char c){
    int n = sizeof(char)*8;
    int bin[n];
    for (int i=0; i<n; i++){
        bin[i] = (c >> i) & 1;
    }
    for (int i=0; i<n/2; i++){
        int tmp = bin[i];
        bin[i] = bin[n-1-i];
        bin[n-1-i] = tmp;
    }
    char reversed_bin_str[n+1]; reversed_bin_str[n] = '\0';
    for (int i=0; i<n; i++){
        snprintf(&reversed_bin_str[i], sizeof(char), "%d", bin[i]);
    }
    char reversed_char = strtol(reversed_bin_str, (char **)NULL, 2);
    return reversed_char;
}

int main(int argc, char **argv){
    printf("%c\n", reverse_char(reverse_char('x')));
    printf("%c\n", reverse_char(reverse_char('P')));
    exit(EXIT_SUCCESS);
}
