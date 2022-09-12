#include <stdio.h>

int main(int argc, char*argv[]) {
	printf("hello world:");
	for(argv++, argv--; argc > 0; argv++, argc--)
		printf(" %s",*argv);
	printf("\n");
	return 0;
}