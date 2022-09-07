//commento fatto con gedit
#include <stdio.h>
#define recv recv1

void recv1(char *s) {
	unsigned char c;
	int i;
	for (i = 0; *s != 0; s++, i = (i + 1) & 0x7, c >>= 1) {
		int bit = (*s == '1');
		if (bit) c |= 0x80;
		if (i == 7)
			printf("%c", c);
	}
}

void recv2(char *s) {
	unsigned char c;
	int i;
	for (i = 0, c = 0; *s != 0; s++, i = (i + 1) & 0x7) {
		int bit = (*s == '1');
		if (bit) c |= 1 << i;
		if (i == 7) {
			printf("%c", c);
			c = 0;
		}
	}
}

void recvr1(char *s) {
  unsigned char c;
  int i;
  for (i = 0; *s != 0; s++, i = (i + 1) & 0x7, c <<= 1) {
    int bit = (*s == '1');
    if (bit) c |= 1;
    if (i == 7)
      printf("%c", c);
  }
}

void recvr2(char *s) {
  unsigned char c;
  int i;
  for (i = 7, c = 0; *s != 0; s++, i = (i - 1) & 0x7) {
    int bit = (*s == '1');
    if (bit) c |= 1 << i;
    if (i == 7) {
      printf("%c", c);
      c = 0;
    }
  }
}
int main(int argc, char *argv[]) {
	recv(argv[1]);
	printf("\n");
}
