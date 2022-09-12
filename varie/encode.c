/* pensieri: leggere e scrivere un carattere alla volta è inefficiente. tenere una stringa di caratteri '0' e '1' per poi usare la strtol per generare il byte è macchinoso. */
/* allego encode.c che mostra come invertire l'ordine dei bit di un byte. */
/* char encodechar(char in) { */
/*   unsigned char out = 0; */
/*   for (int i = 0; i < 8; i++) { */
/*     out >>= 1; */
/*     out |= (in & 0x80); */
/*     in <<= 1; */
/*   } */
/*   return out; */
/* } */
/* (notate che ho dovuto mettere unsigned out se no mi propagava ll segno nello shift a destra.) */
#include <stdio.h>

char encodechar(char in) {
	char out = 0;
	for (int i = 0; i < 8; i++) { //for each bit
		out <<= 1;
		out |= (in & 1);
		in >>= 1;
	}
	return out;
}

void encode(char *s) {
	for (; *s != 0; s++)
		*s = encodechar(*s);
}

void printhex(unsigned char *s) { //raw data
	for (; *s != 0; s++)
		printf("%02x ", *s); //come viene letto..
	printf("\n");
}

int main(int argc, char *argv[]) {
	printf("%s\n", argv[1]);
	printhex(argv[1]);
	encode(argv[1]);
	printhex(argv[1]);
	encode(argv[1]);
	printhex(argv[1]);
	printf("%s\n", argv[1]);
}
