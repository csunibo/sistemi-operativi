#include <stdio.h>

int main(int argc, char *argv[]){
	int buf;	
	FILE *fin = fopen(argv[1], "r");
	FILE *fout = fopen(argv[2], "w");
	while((buf= getc (fin)) != EOF){
		putc (buf, fout);
	}
	fclose (fin);
	fclose (fout);
}
