#include <stdio.h>
 
struct A {
  char a1;
  long a2;
  char a3;
};
 
struct B {
  char b1;
  char b2;
  long b3;
};
 
int main() {
  printf("%d %d\n", sizeof(struct A), sizeof(struct B));
}
