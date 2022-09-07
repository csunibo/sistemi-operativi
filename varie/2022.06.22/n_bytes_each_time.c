#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv){
	if (argc != 2){
		fprintf(stderr, "BAD USAGE");
		exit(EXIT_FAILURE);
	}
	int n=4, i =0; //numero di caratteri che voglio prendere alla volta, e cursore stringa
	char *chunck = (char *)calloc(n+1, sizeof(char));
	for (int i=0; i<strlen(argv[1]); i+=n){ //ci andrebbe il minimo tra i+=n e strlen
		for (int j=0; j<n; j++){
			chunck[j] = *(argv[1] + i + j); //qua bisognerebbe controllare se posso svolgere l'operazione
		}
		printf("%d\n", i);
		printf("chunck %d: %s\n", i/n, chunck);

	}
	free(chunck);
	exit(EXIT_SUCCESS);
}
