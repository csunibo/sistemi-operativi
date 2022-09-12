#include <stdio.h>

void carica(int *n){
	scanf("%d", n);
}

void init(int *inpadd){
	carica(inpadd);
	printf("input value: %d\n", inpadd);
}

int fattoriale(int n){
	int res= 1;
	for (int i=n; i>= 0; i--) res*=i;
}

void stampa(int n, int risultato){
	printf("il risultato di \"input\"(%d) vale (%d)", n, risultato);
}

int main(){
	int input;
	init(&input);
	stampa(input, fattoriale(input));
}
