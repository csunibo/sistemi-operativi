#include<stdio.h>
char *s="#include<stdio.h>%cchar *s=%c%s%c;%cint main() {printf(s,10,34,s,34,10,10);}%c";
int main() {printf(s,10,34,s,34,10,10);}
