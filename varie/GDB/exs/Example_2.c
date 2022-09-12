#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]){
	errno = 0;
	FILE* fp = fopen(argv[1], "r+"); //La funzione fopen in C serve ad aprire un file in varie modalità.
	if (fp == NULL){
		printf("Error: %s\n", strerror(errno));
		getchar();
		exit(1);
	}
	char* word = (char *)(malloc(sizeof(char)*30));
	while (fscanf(fp,"%s",word)>0) { } //from to
	printf("%s", word);
	free(word);
	return 0;
}
/*
The purpose of the program is to read a set of strings from
a file. But the program seg faults. Use gdb to find the
error.
while (fscanf(fp,"%s",word)>0) { } lancia un errore qui, in particolare: __vfscanf_internal (s=0x0, format=0x55555555600e "%s"
//quindi fopen apre male. infatti (gdb) print fp $2 = (FILE *) 0x0
//ho messo quindi da r -> r+. ora mi fornisce un indirizzo di memoria valido.
ma printando word (l'ho dichiarato solo ebbasta) mi esce fuori un null pointer
quindi le assegno memoria con malloc poichè è un pointer, anzichè '\0';
togliendo le virgolette da argv[1], il codice funziona correttamente
*/