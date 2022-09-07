#include <stdio.h>
#include <stdarg.h>

void printEachInt(int n, ...){ //always put mandatory arguments first, followed by "..."
	va_list args;
	//init
	va_start(args, n); //n = last mandatory argument
	for (int i = 0; i < n; i++){
		//applicabile solo per variabili self promoting type => che non fanno casting a tipi con più bit quando fanno operazioni aritmetiche (per evitare overflow)
		//e.g. char a = (30 * 40) / 10; (signed char varia da -128 a 127 eppure durante l'operazione...) => no char, short int, enum
		//https://www.geeksforgeeks.org/integer-promotions-in-c/
		int value = va_arg(args,int); //next call returns the next argument
		//we told va_arg what type the current argument is so it knows how big it is
		printf("%d\n", value);
	}
	va_end(args); //clean up
}

void main(){
	printEachInt(3, 1, 4, 6);
}