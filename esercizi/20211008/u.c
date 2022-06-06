#include <stdio.h>
#include <stdlib.h>
union int2char {
	int i;
	unsigned char c[sizeof(int)];
};

int main(int argc, char *argv[]) {
	union int2char myi2c;
	myi2c.i = atoi(argv[1]);
	printf("%02x:%02x:%02x:%02x\n",
			myi2c.c[0],  myi2c.c[1],  myi2c.c[2],  myi2c.c[3]);
}
