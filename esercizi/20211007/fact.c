#include <stdio.h>
#include <stdlib.h>

unsigned int i;

unsigned int fact(unsigned int i) {
	static int j;
	printf("i %p\n", &i);
	printf("j %p\n", &j);
	return (i==0) ? 1 : i * fact(i - 1);
}

int main(int argc, char *argv[]) {
	printf("i %p\n", &i);
	printf("%d\n", fact(atoi(argv[1])));
}
