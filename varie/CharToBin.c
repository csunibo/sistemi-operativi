#include <stdio.h>
#include <stdlib.h>
char encodechar(char in) {
	char out = 0;
	for (int i = 7; i >= 0; i--) { //for each bit
		out <<= 1;
		out |= (in >> i) & 0x1;
	}
	return out;
}
int main(int argc, char **argv){
    /* if (argc != 2){ */
    /*     fprintf(stderr, "\nBAD USAGE"); */
    /*     exit(EXIT_FAILURE); */
    /* } */
    printf("a == %c\n", encodechar('a'));
    exit(EXIT_SUCCESS);
}
