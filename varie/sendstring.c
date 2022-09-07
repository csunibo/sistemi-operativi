/* per meditare su come spedire e ricevere bit a bit i byte di una stringa vi allego due file sendstring.c e recvstring.c. Ogni sorgente contiene 2 modi per trasmettere o ricevere i bit a partire dal bit meno significativo e 2 a partire da quello più significatico. Con la #define in testa si puó scegliere la funzione. Ovviamente funzionano solo se l'ordine dei bit in trasmissione è coerente con quello in ricezione. */
#include <stdio.h>

#define charsend charsend1
void charsend1(unsigned char c) {
	for (int i = 0; i < 8; i++, c >>= 1)
		printf((c & 1) ? "1" : "0");
}

void charsend2(unsigned char c) {
	for (int i = 0; i < 8; i++)
		printf((c & (1 << i)) ? "1" : "0");
}

void charsendr1(unsigned char c) {
	for (int i = 0; i < 8; i++, c <<= 1)
		printf((c & 0x80) ? "1" : "0");
}

void charsendr2(unsigned char c) {
	for (int i = 7; i >= 0; i--)
		printf((c & (1 << i)) ? "1" : "0");
}

void send(char *s) {
	for (; *s != 0; s++)
		charsend(*s);
	charsend(0);
}

int main(int argc, char *argv[]) {
	send(argv[1]);
	printf("\n");
}
