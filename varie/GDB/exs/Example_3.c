#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char* argv[]){
	printf("%ld \n", INT_MAX);
	int n = INT_MAX;
	int A[n];
	int i = 0;
	while (i<n)
		A[i] = rand()%10;
	return EXIT_SUCCESS;
}
/*
The purpose of the program is to allocate an array of ints
and initialize them to a random number. But the program seg
faults. Use gdb to find the error.

il programma entra in segfault quando cerco di allocare A[n] celle, con n=MAXINT.
Se anzichè n metto 10 => set var n = 10 tutto funziona correttamente
*/