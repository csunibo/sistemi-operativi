#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	int i=0,n=1000;
	char name[32];
	char* A[n];
	for (int i=0; i<n; i++){
		A[i] = (char *)(malloc(sizeof(char)*200));
	}
	FILE* infile = fopen(argv[1],"r");
	while (fscanf(infile,"%s", name) > 0)
		strcpy(A[i++],name);
	/*shift all elements to right by 1*/
	for (int j=i; j>0; j--){
		//A[i+1] = A[i];
		printf("%s\n", A[i]);
	}
}

//Ai puntatori char va sempre allocata memoria!!! => per ogni stringa nell'array allocarle memoria!!!